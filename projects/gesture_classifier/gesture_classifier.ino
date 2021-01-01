#include <Arduino_LSM9DS1.h>

#include <TensorFlowLite.h>
#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/micro_error_reporter.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/schema/schema_generated.h>
#include <tensorflow/lite/version.h>

#include "model.h"

const float accelerationThreshold = 1.9; // threshold of significant in G's
const int numSamples = 119;
const int ledPin = 2;

int samplesRead = numSamples;

// global variables used for TensorFlow Lite (Micro)
tflite::MicroErrorReporter tflErrorReporter;

// pull in all the TFLM ops, you can remove this line and
// only pull in the TFLM ops you need, if would like to reduce
// the compiled size of the sketch.
tflite::AllOpsResolver tflOpsResolver;

const tflite::Model* tflModel = nullptr;
tflite::MicroInterpreter* tflInterpreter = nullptr;
TfLiteTensor* tflInputTensor = nullptr;
TfLiteTensor* tflOutputTensor = nullptr;

// Create a static memory buffer for TFLM, the size may need to
// be adjusted based on the model you are using
constexpr int tensorArenaSize = 8 * 1024;
byte tensorArena[tensorArenaSize];

// array to map gesture index to a name
const char* GESTURES[] = {
  "cw", "ccw", "fwd" // classify digits 0 to 4 written in the air from top right to bottom left
};

#define NUM_GESTURES (sizeof(GESTURES) / sizeof(GESTURES[0]))

void setup() {
    Serial.begin(9600);
    while (!Serial); // comment this line if you're running without the USB cable
  
    // initialize the IMU
    if (!IMU.begin()) {
        Serial.println("Failed to initialize IMU!");
        while (1);
    }
  
    // print out the samples rates of the IMUs
    Serial.print("Accelerometer sample rate = ");
    Serial.print(IMU.accelerationSampleRate());
    Serial.println(" Hz");
    Serial.print("Gyroscope sample rate = ");
    Serial.print(IMU.gyroscopeSampleRate());
    Serial.println(" Hz");
    Serial.println();
  
    // get the TFL representation of the model byte array
    tflModel = tflite::GetModel(model);
    if (tflModel->version() != TFLITE_SCHEMA_VERSION) {
        Serial.println("Model schema mismatch!");
        while (1);
    }
    Serial.println("1");
    // Create an interpreter to run the model
    tflInterpreter = new tflite::MicroInterpreter(tflModel, tflOpsResolver, tensorArena, tensorArenaSize, &tflErrorReporter);
      Serial.println("2");
    // Allocate memory for the model's input and output tensors
    tflInterpreter->AllocateTensors();
      Serial.println("3");
    // Get pointers for the model's input and output tensors
    tflInputTensor = tflInterpreter->input(0);
    tflOutputTensor = tflInterpreter->output(0);
        Serial.println("4");
    pinMode(ledPin, OUTPUT);
    Serial.println("finished initialization");
}

void loop() {
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
          // Loop through the output tensor values from the model
          for (int i = 0; i < NUM_GESTURES; i++) {
              Serial.print(GESTURES[i]);
              Serial.print(": ");
              Serial.println(tflOutputTensor->data.f[i], 6);
          }
          Serial.println();
      }    
  }
}
