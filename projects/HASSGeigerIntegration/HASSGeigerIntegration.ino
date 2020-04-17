#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <SoftwareSerial.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


#ifndef STASSID
#define STASSID "" // Replace with your WIFI SSID
#define STAPSK  "" // Replace with your WIFI password
#endif

#define SKETCHPASS "thepass" // Replace with your WIFI password
#define MQTT_SERVER "192.168.1.33" // replace with yout MQTT sderver


const char* ssid = STASSID;
const char* password = STAPSK;

SoftwareSerial geigerCounter(13, 15);
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, 1883);
Adafruit_MQTT_Publish radiationTopic = Adafruit_MQTT_Publish(&mqtt, "ha/radiation");


void setup() {
    Serial.begin(9600);
    geigerCounter.begin(9600);
    Serial.println("Booting");
    startOTA(); // comment this line and  if you dont't want OTA
    pinMode(LED_BUILTIN, OUTPUT);
    MQTT_connect();
}

void loop() {
    ArduinoOTA.handle(); // comment this line if you dont't want OTA
    if (geigerCounter.available()) {
        handleNewReading(geigerCounter.parseInt());
    }
}

void handleNewReading(int reading) {
    if (reading == 0) {
        return;// parseInt bug
    }
    Serial.println(reading);
    Serial.println(String(reading));
    char buf[50]; //make this the size of the String
    String message = "{\"radiation\": " + String(reading) + "}";
    Serial.println(message);
    message.toCharArray(buf, message.length() + 1);
    radiationTopic.publish(buf);
}

void MQTT_connect() {
    int8_t ret;
    if (mqtt.connected()) {
        return;
    }
    Serial.print("Connecting to MQTT... ");
    uint8_t retries = 3;
    while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
         Serial.println(mqtt.connectErrorString(ret));
         Serial.println("Retrying MQTT connection in 5 seconds...");
         mqtt.disconnect();
         delay(5000);  // wait 5 seconds
         retries--;
         if (retries == 0) {
           while (1);
         }
    }
    Serial.println("MQTT Connected!");
}

void startOTA() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("Connection Failed! Rebooting...");
      delay(5000);
      ESP.restart();
    }
    ArduinoOTA.setPassword(SKETCHPASS);
    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
          type = "sketch";
        } else { // U_FS
          type = "filesystem";
        }
        Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) {
          Serial.println("Auth Failed");
        } else if (error == OTA_BEGIN_ERROR) {
          Serial.println("Begin Failed");
        } else if (error == OTA_CONNECT_ERROR) {
          Serial.println("Connect Failed");
        } else if (error == OTA_RECEIVE_ERROR) {
          Serial.println("Receive Failed");
        } else if (error == OTA_END_ERROR) {
          Serial.println("End Failed");
        }
    });
    ArduinoOTA.begin();
    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}
