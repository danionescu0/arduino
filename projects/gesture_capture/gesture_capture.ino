/*
*  This example uses the on-board IMU to start reading acceleration and gyroscope
*  data from on-board IMU and prints it to the Serial Monitor for one second
*  when the significant motion is detected, it also logs the data to a SD card
*  Saves each gesture data to a SD card in a separate file "1.csv", "2.csv" etc
*  based on example from https://github.com/arduino/ArduinoTensorFlowLiteTutorials
*  To record the next gesture swipe right with your hand in front of the board, it uses
*  the on board gesture detector to detect GESTURE_RIGHT and to record in the next csv file
*  
* PINOUT
* 
* CARD READER 5V variant
* MOSI   -  pin 11  
* MISO   -  pin 12
* CLK    -  pin 13
* CS     -  pin 4
* GND    -  GND
* VCC    -  VIN
* 
* LED
* D2     -  pin through a 220ohms resistor
* GND    -  GND
*/

#include <SPI.h>
#include <SD.h>
#include <Arduino_LSM9DS1.h>
#include <Arduino_APDS9960.h>

const float accelerationThreshold = 1.9; // threshold of significant in G's
const int numSamples = 119;
const int sdChipSelect = 4;
const int ledPin = 2;

int samplesRead = numSamples;
int currentGesture;
byte gestureNr = 1;
byte fileNr = 1;
byte printBufferCursor = 0;
String printBuffer[9] = {};
File datafile;


void setup() {
    Serial.begin(9600);
    while (!Serial); // comment this line when the device is not connected with the USB cable to the computer
    if (!IMU.begin()) {
        Serial.println("Failed to initialize IMU!");
        while (1000);
    }
    if (!SD.begin(sdChipSelect)) {
        Serial.println("Card failed, or not present");
        while (1000);
    }
    if (!APDS.begin()) {
       Serial.println("Error initializing APDS9960 sensor.");
       while (1000); // Stop forever
    }    
    pinMode(ledPin, OUTPUT);
    //remove files on card
    for (byte i=0;i<=10;i++) {
        SD.remove(String(i) + ".csv");
    }
    prepareNextFile();
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
        processGesture();
        // if it's above the threshold reset the sample read count
        if (aSum >= accelerationThreshold) {
            samplesRead = 0;
            digitalWrite(ledPin, HIGH);
            break;
        }        
    }
  
    // check if the all the required samples have been read since
    // the last time the significant motion was detected
    while (samplesRead < numSamples) {
        if (!(IMU.accelerationAvailable() && IMU.gyroscopeAvailable())) {
            break;
        }
        processGesture();
        IMU.readAcceleration(aX, aY, aZ);
        IMU.readGyroscope(gX, gY, gZ);
        samplesRead++;
        print(String(aX, 3) + "," + String(aY, 3) + "," + String(aZ, 3) + "," + String(gX, 3) + "," + String(gY, 3) + "," + String(gZ, 3) + "\n");
        if (samplesRead == numSamples) { // if nr of samplex is aquired print a newline and flush to sd card
            print("\n");
            flushToSdCard();
            gestureNr++;
            digitalWrite(ledPin, LOW);
            Serial.println("Waiting for gesture nr " + String(gestureNr));            
        }
    }   
}

void print(String data) {    
    Serial.print(data);
    printBuffer[printBufferCursor] += data;
    if (printBuffer[printBufferCursor].length() > 1000) {
        printBufferCursor++;
    }
}

void flushToSdCard() {
     for(byte i = 0;i<=printBufferCursor;i++) {
        datafile.print(printBuffer[i]);
     }
     for(byte i = 0;i<=printBufferCursor;i++) {
        printBuffer[i] = "";
     }
     printBufferCursor = 0;
}

/**
 * If the file is open closes current datafile
 * create new file 1.csv then 2.csv etc
 * append CSV header to it
 */
void prepareNextFile() {    
    Serial.println("Writng to file: " + String(fileNr) + ".csv");
    if (datafile) {
        Serial.println("closing datafile");
        datafile.close();
        delay(100);
    }
    datafile = SD.open(String(fileNr) + ".csv", FILE_WRITE);
    delay(100);
    print("aX,aY,aZ,gX,gY,gZ\n");
    fileNr++;
    gestureNr = 0;
    Serial.println("Waiting for gesture nr " + String(gestureNr));  
}

/**
 * Check if right gesture is available switch to the next file
 * also flashes the led two times
 */
void processGesture() {
    if (!APDS.gestureAvailable()) {
        return;
    }
    if (APDS.readGesture() == GESTURE_RIGHT) {
        digitalWrite(ledPin, HIGH);
        delay(150);
        digitalWrite(ledPin, LOW);
        delay(150);
        digitalWrite(ledPin, HIGH);
        delay(300);
        digitalWrite(ledPin, LOW);        
        prepareNextFile();
    }
}
