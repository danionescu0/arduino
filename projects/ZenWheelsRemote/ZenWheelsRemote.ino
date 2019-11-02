/**
 * Libraries:
 * https://github.com/jrowberg/i2cdevlib
 * https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050
 */
#include "I2Cdev.h"
#include <Adafruit_NeoPixel.h>
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
#include <SoftwareSerial.h>

const int MAX_ANGLE = 45;
const byte commandStering = 129;
const byte commandSpeed = 130;


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

unsigned long lastPrintTime, lastMoveTime = 0;

SoftwareSerial BTserial(10, 11);
MPU6050 mpu;


void setup() 
{
  Serial.begin(9600);
  BTserial.begin(38400);
  Serial.println("Program started");
  initialization = initializeGyroscope(); 
}

void loop() {
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
        steer(ypr[0] * 180/M_PI, ypr[1] * 180/M_PI, ypr[2] * 180/M_PI);
    }  
}

/*
 * Receives angle from 0 to 180 where 0 is max left and 180 is max right
 * Receives speed from -90 to 90 where -90 is max backwards and 90 is max forward
 */
void moveZwheelsCar(byte angle, int speed) 
{
    if (millis() - lastMoveTime < 100) {
        return;
    }
    byte resultAngle;
    int resultSpeed;
    if (angle >= 90) {
      resultAngle = map(angle, 91, 180, 1, 63);    
    } else if (angle < 90) {
      resultAngle = map(angle, 1, 90, 63, 126);
    }
    if (speed > 0) {
      resultSpeed = map(speed, 0, 90, 0, 63);    
    } else if (speed < 0) {
      resultSpeed = map(speed, 0, -90, 126, 64);
    }
    Serial.print("actualAngle=");Serial.print(angle);Serial.print("; ");
    Serial.print("actualSpeed=");Serial.print(resultSpeed);Serial.println("; ");
    BTserial.write(commandStering);    
    BTserial.write(resultAngle); 
    BTserial.write(commandSpeed);
    BTserial.write((byte) resultSpeed); 
    lastMoveTime = millis();
}

void steer(int x, int y, int z)
{  
    x = constrain(x, -1 * MAX_ANGLE, MAX_ANGLE);
    y = constrain(y, -1 * MAX_ANGLE, MAX_ANGLE);
    z = constrain(z, -MAX_ANGLE, MAX_ANGLE);
    int angle = map(y, -MAX_ANGLE, MAX_ANGLE, 0, 180);
    int speed = map(z, -MAX_ANGLE, MAX_ANGLE, 90, -90);
    printDebug(x, y, z, angle, speed);
    moveZwheelsCar(angle, speed);
}

void printDebug(int x, int y, int z, int angle, int speed)
{
    if (millis() - lastPrintTime < 1000) {
        return;
    }
    Serial.print("z=");Serial.print(x);Serial.print("; ");
    Serial.print("y=");Serial.print(y);Serial.print("; ");
    Serial.print("z=");Serial.print(z);Serial.print("; ");
    Serial.print("angle=");Serial.print(angle);Serial.print("; ");
    Serial.print("speed=");Serial.print(speed);Serial.println("; ");
    lastPrintTime = millis();
}

bool initializeGyroscope() 
{
    Wire.begin();
    mpu.initialize();
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
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

boolean hasFifoOverflown(int mpuIntStatus, int fifoCount) 
{
    return mpuIntStatus & 0x10 || fifoCount == 1024;
}
