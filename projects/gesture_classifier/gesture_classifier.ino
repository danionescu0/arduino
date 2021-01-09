/**
 * Detects gestures that wore previously trained with gesture_capture.ino
 * After an accelerationThreshold is reached, the sketch begin recording numSamples (119)
 * and then feeds in into TensorFlowLite to predict the gesture.
 * The LED is used to signal that the sketch is recording a gesture
 * If you want to use your own trained data, replace #include "digits_model.h" with your own, then replace const char* GESTURES[] 
 * with your own trained gestures
 *                           
 * Arduino IDE vs: 1.8.12
 * 
 * Libraries: 
 * Arduino_LSM9DS1 (version 1.0.0)
 * TensorFlowLite (version 2.1.0 ALPHA precompiled)
 * ArduinoBLE (version 1.1.3)
 * 
 * Board version in arduino IDE:
 * Arduino mbed-enabled board vs 1.1.6 (tested with this version, does not work with other versions)
 * 
 * PINOUT
 * 
 * LED
 * D2     -  pin through a 220ohms resistor
 * GND    -  GND
 * 
 * Trained models:
 * digits_model.h -> uses gestures 0,1,2,3,4 (draw it in the air)
 * remote1.mode.h -> uses gestures cw,ccw,fwd (clockwise, counter clockwise, forward in the air)
 */
#include <Arduino_LSM9DS1.h>
#include <ArduinoBLE.h>
#include <TensorFlowLite.h>
#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/micro_error_reporter.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/schema/schema_generated.h>
#include <tensorflow/lite/version.h>

#include "remote1_model.h"

const float accelerationThreshold = 1.9; // threshold of significant in G's
const int numSamples = 119;
const int ledPin = 2;

int samplesRead = numSamples;

// global variables used for TensorFlow Lite (Micro)
tflite::MicroErrorReporter tflErrorReporter;
tflite::AllOpsResolver tflOpsResolver;

const tflite::Model* tflModel = nullptr;
tflite::MicroInterpreter* tflInterpreter = nullptr;
TfLiteTensor* tflInputTensor = nullptr;
TfLiteTensor* tflOutputTensor = nullptr;
constexpr int tensorArenaSize = 8 * 1024;
byte tensorArena[tensorArenaSize];

BLEService gestureService("19B10000-E8F2-537E-4F6C-D104768A1214"); 
// BLE LED Switch Characteristic - custom 128-bit UUID, read 
BLEByteCharacteristic gestureCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
BLEDevice centralBLE;

// array to map gesture index to a name
const char* GESTURES[] = {
  "cw", "ccw", "fwd" 
};

#define NUM_GESTURES (sizeof(GESTURES) / sizeof(GESTURES[0]))


void setup() {
    Serial.begin(9600);
    while (!Serial); // comment this line if you're running without the USB cable  
    if (!IMU.begin()) {
        Serial.println("Failed to initialize IMU!");
        while (1);
    }
  
    // print out the samples rates of the IMUs
    Serial.print("Accelerometer sample rate = ");
    Serial.print(IMU.accelerationSampleRate());
    Serial.print("Gyroscope sample rate = ");
    Serial.print(IMU.gyroscopeSampleRate());
    Serial.println();
  
    // get the TFL representation of the model byte array
    tflModel = tflite::GetModel(model);
    if (tflModel->version() != TFLITE_SCHEMA_VERSION) {
        Serial.println("Model schema mismatch!");
        while (1);
    }
    // Create an interpreter to run the model
    tflInterpreter = new tflite::MicroInterpreter(tflModel, tflOpsResolver, tensorArena, tensorArenaSize, &tflErrorReporter);
    // Allocate memory for the model's input and output tensors
    tflInterpreter->AllocateTensors();
    // Get pointers for the model's input and output tensors
    tflInputTensor = tflInterpreter->input(0);
    tflOutputTensor = tflInterpreter->output(0);
    pinMode(ledPin, OUTPUT);
    initBLE();
    Serial.println("finished initialization");
}

void loop() {
  //accept BLE connection
  centralBLE = BLE.central();
  float aX, aY, aZ, gX, gY, gZ;
  // wait for significant motion
  while (samplesRead == numSamples) {
      if (!IMU.accelerationAvailable()) {
          break;
      }
      // read the acceleration data and sum up the absolutes
      IMU.readAcceleration(aX, aY, aZ);
      float aSum = fabs(aX) + fabs(aY) + fabs(aZ);
      // check if it's above the threshold
      if (aSum >= accelerationThreshold) {
        Serial.println("Begin reading gesture");
        samplesRead = 0;
        digitalWrite(ledPin, HIGH);
        break;
      }      
  }

  // check if the all the required samples have been read since
  // the last time the significant motion was detected
  while (samplesRead < numSamples) {
    // check if new acceleration AND gyroscope data is available
      if (!(IMU.accelerationAvailable() && IMU.gyroscopeAvailable())) {
          break;
      }
      // read the acceleration and gyroscope data
      IMU.readAcceleration(aX, aY, aZ);
      IMU.readGyroscope(gX, gY, gZ);

      // normalize the IMU data between 0 to 1 and store in the model's input tensor
      tflInputTensor->data.f[samplesRead * 6 + 0] = (aX + 4.0) / 8.0;
      tflInputTensor->data.f[samplesRead * 6 + 1] = (aY + 4.0) / 8.0;
      tflInputTensor->data.f[samplesRead * 6 + 2] = (aZ + 4.0) / 8.0;
      tflInputTensor->data.f[samplesRead * 6 + 3] = (gX + 2000.0) / 4000.0;
      tflInputTensor->data.f[samplesRead * 6 + 4] = (gY + 2000.0) / 4000.0;
      tflInputTensor->data.f[samplesRead * 6 + 5] = (gZ + 2000.0) / 4000.0;
      samplesRead++;
      if (samplesRead == numSamples) {
          // Run inferencing
          digitalWrite(ledPin, LOW);
          Serial.println("Finished reading gesture");
          TfLiteStatus invokeStatus = tflInterpreter->Invoke();
          if (invokeStatus != kTfLiteOk) {
              Serial.println("Invoke failed!");
              while (1);
              return;
          }
          transmitData();
      }    
  }
}

void transmitData() {
    // Loop through the output tensor values from the model
    int maxPosition = 0;
    float maxValue = 0;
    for (int i = 0; i < NUM_GESTURES; i++) {
        Serial.print(GESTURES[i]);Serial.print(": ");
        Serial.println(tflOutputTensor->data.f[i], 6);
        if (maxValue < tflOutputTensor->data.f[i]) {
            maxValue = tflOutputTensor->data.f[i];
            maxPosition = i;
        }
    }
    Serial.println("Highest probability:");
    Serial.print(maxPosition);Serial.print(": ");
    Serial.println(maxValue, 6);    
    Serial.println();
    if (centralBLE) {    
        if (centralBLE.connected()) {
           gestureCharacteristic.writeValue(maxPosition);
        }
    }
}

void initBLE() {
    if (!BLE.begin()) {
        Serial.println("starting BLE failed!");
        while (1);
    }
    // set advertised local name and service UUID:
    BLE.setLocalName("GESTURE");
    BLE.setAdvertisedService(gestureService);
    gestureService.addCharacteristic(gestureCharacteristic);
    BLE.addService(gestureService);
    // set the initial value for the characeristic:
    gestureCharacteristic.writeValue(0); 
    BLE.advertise(); 
}
