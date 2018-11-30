/**
 * Libraries:
 * https://github.com/adafruit/Adafruit_NeoPixel
 * https://github.com/jrowberg/i2cdevlib
 * https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050
 */
#include "I2Cdev.h"
#include <Adafruit_NeoPixel.h>
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

#define NEOPIXED_CONTROL_PIN 6
#define NUM_LEDS 24

const int MAX_ANGLE = 45;
const int LED_OFFSET = 12;

MPU6050 mpu;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, NEOPIXED_CONTROL_PIN, NEO_RBG + NEO_KHZ800);
unsigned long lastPrintTime = 0;


bool initialization = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high

void setup() 
{
    Serial.begin(9600);
    Serial.println("Program started");
    initialization = initializeGyroscope();  
    strip.begin(); 
}

void loop() 
{
    if (!initialization) {
        return;
    }
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();
    fifoCount = mpu.getFIFOCount();
    if (hasFifoOverflown(mpuIntStatus, fifoCount)) {
        mpu.resetFIFO();
        return;
    }
    if (mpuIntStatus & 0x02) {
        while (fifoCount < packetSize) {
            fifoCount = mpu.getFIFOCount();
        }
        mpu.getFIFOBytes(fifoBuffer, packetSize);        
        fifoCount -= packetSize;
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        redrawLeds(ypr[0] * 180/M_PI, ypr[1] * 180/M_PI, ypr[2] * 180/M_PI);
    }
}

boolean hasFifoOverflown(int mpuIntStatus, int fifoCount) 
{
    return mpuIntStatus & 0x10 || fifoCount == 1024;
}

void redrawLeds(int x, int y, int z)
{  
    x = constrain(x, -1 * MAX_ANGLE, MAX_ANGLE);
    y = constrain(y, -1 * MAX_ANGLE, MAX_ANGLE);
    if (y < 0 and z > 0) {
        lightLeds(y, z, 0, 5, 0, 89);      
    } else if (y < 0 and z < 0) {
        lightLeds(y, z, 6, 12, 89, 0);  
    } else if (y > 0 and z < 0) {
        lightLeds(y, z, 13, 19, 0, 89);     
    } else if (y > 0 and z > 0) {
        lightLeds(y, z, 20, 24, 89, 0);     
    }
}


void lightLeds(int x, int y, int fromLedPosition, int toLedPosition, int fromAngle, int toAngle) 
{
    double angle = (atan((double) abs(x) / (double) abs (y)) * 4068) / 71;
    int ledNr = map(angle, fromAngle, toAngle, fromLedPosition, toLedPosition);
    printDebug(x, y, ledNr, angle);  
    uint32_t color;

    for (int i=0; i < NUM_LEDS; i++) {
        color = strip.Color(0, 0, 0);
        if (i == ledNr) {
           color = strip.Color(0, 180, 0);
        } else if (i == ledNr - 1) {
           color = strip.Color(0, 5, 0);
        }
        strip.setPixelColor(normalizeLedPosition(i), color); 
        strip.show();
    }  
}

int normalizeLedPosition(int position)
{
    if (NUM_LEDS > position + LED_OFFSET) {
        return position + LED_OFFSET;
    }

    return position + LED_OFFSET - NUM_LEDS;
}

void printDebug(int y, int z, int lightLed, int angle)
{
    if (millis() - lastPrintTime < 500) {
        return;
    }
    Serial.print("a=");Serial.print(angle);Serial.print("; ");
    Serial.print("ll=");Serial.print(lightLed);Serial.print("; ");
    Serial.print("y=");Serial.print(y);Serial.print("; ");
    Serial.print("z=");Serial.print(z);Serial.println("; ");
    lastPrintTime = millis();
}


bool initializeGyroscope() {
    Wire.begin();
    TWBR = 24;  
    mpu.initialize();
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788);
    if (devStatus != 0) {
        Serial.print(F("DMP Initialization failed (code "));Serial.println(devStatus);
        return false;
    }
    mpu.setDMPEnabled(true);
    Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
    attachInterrupt(0, dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();
    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    packetSize = mpu.dmpGetFIFOPacketSize();
    
    return true;
}

void dmpDataReady() 
{
    mpuInterrupt = true;
}

