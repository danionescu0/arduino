#include "I2Cdev.h"
#include <Adafruit_NeoPixel.h>
#include <SparkFun_APDS9960.h>
#include "Wire.h"

#define NEOPIXED_CONTROL_PIN 6
#define NUM_LEDS 24
#define APDS9960_INT 2
#define LED_SPEED_STEP_INTERVAL 100
#define COLOR_INTENSITY 100


Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, NEOPIXED_CONTROL_PIN, NEO_RBG + NEO_KHZ800);
SparkFun_APDS9960 apds = SparkFun_APDS9960();

struct color
  {
      byte red;
      byte green;
      byte blue;    
  }; 

color currentColor;
unsigned long lastLedChangeTime = 0;
short ledDirection = 0;
byte ledPosition = 0;
int isr_flag = 0;


void setup() 
{
    Serial.begin(9600);
    Serial.println("Program started");
    strip.begin(); 
    pinMode(APDS9960_INT, INPUT);
    attachInterrupt(0, interruptRoutine, FALLING);
    if ( apds.init() && apds.enableGestureSensor(true)) {
         Serial.println("APDS-9960 initialization complete");
    } else {
        Serial.println("Something went wrong during APDS-9960 init!");
    }
    lastLedChangeTime = millis();
    currentColor = {150, 0, 0};
    Serial.println("Init succesfully");
}

void loop() 
{
    if( isr_flag == 1 ) {
      detachInterrupt(0);
      handleGesture();
      isr_flag = 0;
      attachInterrupt(0, interruptRoutine, FALLING);
    }
    animateLeds();
}

void interruptRoutine() 
{
    isr_flag = 1;
}

void handleGesture() {
    if ( !apds.isGestureAvailable() ) {
        return;
    }
    switch ( apds.readGesture() ) {
      case DIR_UP:
        Serial.println("UP");
        toggleColor();
        break;
     case DIR_DOWN:
        Serial.println("DOWN");
        toggleColor();
        break;        
      case DIR_LEFT:
        ledDirection = 1;
        Serial.println("LEFT");
        break;
      case DIR_RIGHT:
        ledDirection = -1; 
        Serial.println("RIGHT");
        break;
      case DIR_NEAR:
        ledDirection = 0;
        Serial.println("NEAR");
        break;
    }
}

void toggleColor()
{
    if (currentColor.red > 0 ) {
        currentColor = {0, COLOR_INTENSITY, 0};
    } else if (currentColor.green > 0 ) {
        currentColor = {0, 0, COLOR_INTENSITY};
    } else {
        currentColor = {COLOR_INTENSITY, 0, 0};
    }
}

void animateLeds() 
{
    if (millis() - lastLedChangeTime < LED_SPEED_STEP_INTERVAL) {
        return;
    }
    uint32_t color;
    ledPosition = ledPosition == 0 && ledDirection < 0 ? NUM_LEDS : ledPosition;
    ledPosition += ledDirection;

    for (int i=0; i < NUM_LEDS; i++) {
        color = strip.Color(0, 0, 0);
        if (i == ledPosition) {
           color = strip.Color(currentColor.red, currentColor.green, currentColor.blue);
        } else if (i == ledPosition - 1) {
           color = strip.Color(currentColor.red, currentColor.green, currentColor.blue);
        }
        strip.setPixelColor(i, color); 
        strip.show();
    }  
    ledPosition = ledPosition == NUM_LEDS ? 0: ledPosition;
    lastLedChangeTime = millis();
}
