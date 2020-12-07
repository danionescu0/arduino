/*
  This example uses the on-board IMU to start reading acceleration and gyroscope
  data from on-board IMU and prints it to the Serial Monitor for one second
  when the significant motion is detected.
  Transmits data over serial, and over a secondary serial, i've used a HC-10 communication module
  based on example from https://github.com/arduino/ArduinoTensorFlowLiteTutorials
*/

#include <Arduino_LSM9DS1.h>

UART transmit (digitalPinToPinName(5), digitalPinToPinName(6), NC, NC);
const float accelerationThreshold = 1.9; // threshold of significant in G's
const int numSamples = 119;

int samplesRead = numSamples;

void setup() {
  Serial.begin(9600);
  transmit.begin(9600);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  // print the header
  Serial.println("aX,aY,aZ,gX,gY,gZ");transmit.print("aX,aY,aZ,gX,gY,gZ");
}

void loop() {
    float aX, aY, aZ, gX, gY, gZ;
  
    // wait for significant motion
    while (samplesRead == numSamples) {
      if (IMU.accelerationAvailable()) {
        // read the acceleration data
        IMU.readAcceleration(aX, aY, aZ);
        // sum up the absolutes
        float aSum = fabs(aX) + fabs(aY) + fabs(aZ);
  
        // if it's above the threshold reset the sample read count
        if (aSum >= accelerationThreshold) {
          samplesRead = 0;
          break;
        }
      }
    }
  
    // check if the all the required samples have been read since
    // the last time the significant motion was detected
    while (samplesRead < numSamples) {
      if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
        IMU.readAcceleration(aX, aY, aZ);
        IMU.readGyroscope(gX, gY, gZ);
        samplesRead++;
        printRowInCSV(aX, aY, aZ, gX, gY, gZ);
        if (samplesRead == numSamples) { // if nr of samplex is aquired print a newline
          Serial.println();transmit.println();
        }
      }
    }
}

void printRowInCSV(float aX, float aY, float aZ, float gX, float gY, float gZ) {
      Serial.print(aX, 3);transmit.print(aX,3);
      Serial.print(',');transmit.print(',');
      Serial.print(aY, 3);transmit.print(aY, 3);
      Serial.print(',');transmit.print(',');
      Serial.print(aZ, 3);transmit.print(aZ, 3);
      Serial.print(',');transmit.print(',');
      Serial.print(gX, 3);transmit.print(gX, 3);
      Serial.print(',');transmit.print(',');
      Serial.print(gY, 3);transmit.print(gY, 3);
      Serial.print(',');transmit.print(',');
      Serial.print(gZ, 3);transmit.print(gZ, 3);
      Serial.println();transmit.println();

}
