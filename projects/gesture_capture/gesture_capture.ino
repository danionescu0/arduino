/*
*  This example uses the on-board IMU to start reading acceleration and gyroscope
*  data from on-board IMU and prints it to the Serial Monitor for one second
*  when the significant motion is detected, it also logs the data to a SD card
*  Saves each gesture data to a SD card in a separate file "1.csv", "2.csv" etc
*  based on example from https://github.com/arduino/ArduinoTensorFlowLiteTutorials
*  
* PINOUT
* MOSI   -  pin 11
* MISO   -  pin 12
*  CLK    -  pin 13
*  CS     -  pin 4
*  GND    -  GND
*  VCC    -  VIN
 */

#include <SPI.h>
#include <SD.h>
#include <Arduino_LSM9DS1.h>


const float accelerationThreshold = 1.9; // threshold of significant in G's
const int numSamples = 119;
const int sdChipSelect = 4;

int samplesRead = numSamples;
byte gestureNr = 1;
String printBuffer = "";
File datafile;


void setup() {
    Serial.begin(9600);
    while (!Serial);
  
    if (!IMU.begin()) {
        Serial.println("Failed to initialize IMU!");
        while (1000);
    }
    if (!SD.begin(sdChipSelect)) {
          Serial.println("Card failed, or not present");
          while (1000);
    }
    //remove files on card
    for (byte i=0;i<=10;i++) {
        SD.remove(String(i) + ".csv");
    }
    datafile = SD.open("1.csv", FILE_WRITE);
    // print CSV header
    Serial.println("Waiting for gesture nr 1");
    print("aX,aY,aZ,gX,gY,gZ\n");
}

void loop() {
    float aX, aY, aZ, gX, gY, gZ;
  
    // wait for significant motion
    while (samplesRead == numSamples) {
        if (!IMU.accelerationAvailable()) {
            break;
        }
        IMU.readAcceleration(aX, aY, aZ);
        // sum up the absolutes
        float aSum = fabs(aX) + fabs(aY) + fabs(aZ);
 
        // if it's above the threshold reset the sample read count
        if (aSum >= accelerationThreshold) {
            samplesRead = 0;
            break;
        }        
    }
  
    // check if the all the required samples have been read since
    // the last time the significant motion was detected
    while (samplesRead < numSamples) {
        if (!(IMU.accelerationAvailable() && IMU.gyroscopeAvailable())) {
            break;
        }
        IMU.readAcceleration(aX, aY, aZ);
        IMU.readGyroscope(gX, gY, gZ);
        samplesRead++;
        print(String(aX, 3) + "," + String(aY, 3) + "," + String(aZ, 3) + "," + String(gX, 3) + "," + String(gY, 3) + "," + String(gZ, 3) + "\n");
        if (samplesRead == numSamples) { // if nr of samplex is aquired print a newline
            print("\n");
            flushToSdCard();
            writeOnNextFile();
            Serial.println("Waiting for gesture nr " + String(gestureNr));            
        }
    }   
}

void print(String data) {    
    Serial.print(data);
    printBuffer += data;
    if (printBuffer.length() > 1000) {
        flushToSdCard();
    }
}

void flushToSdCard() {
     datafile.print(printBuffer);
     printBuffer = "";
}

void writeOnNextFile() {    
    datafile.close();
    gestureNr++;
    datafile = SD.open(String(gestureNr) + ".csv", FILE_WRITE);
}
