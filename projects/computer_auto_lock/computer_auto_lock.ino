/* 
 *  Libraries used by this project:
 *  
 *  GOFi2cOLED: https://github.com/hramrach/GOFi2cOLED
 *  Adafruit_MLX90614: https://github.com/adafruit/Adafruit-MLX90614-Library
 */
#include "Keyboard.h"
#include <Wire.h>
#include <GOFi2cOLED.h>
#include "Ultrasonic.h"


GOFi2cOLED GOFoled;
Ultrasonic ultrasonic(12,13);

const byte distancePot = A0;
const byte timerPot = A1;
const float percentMaxDistanceChangedAllowed = 25;
int actualDistance;
unsigned long maxDistanceDetectionTime;
bool lockTimerStarted = false;

void setup() 
{
  Serial.begin(9600);
  Keyboard.begin();
  initializeDisplay();
}

void loop() 
{
  clearDisplay();
  actualDistance = getActualDistance();
  writeStatusData();
  doDisplay();
  if (!lockTimerStarted && shouldEnableLockTimer()) {
     lockTimerStarted = true;
     maxDistanceDetectionTime = millis();
     Serial.println("lock timer begin");
  } else if (!shouldEnableLockTimer()){
     Serial.println("lock timer disabled");
     lockTimerStarted = false;
  }
  if (shouldLockScreen()) {
     lockScreen();
     Serial.println("Lock screen");
     delay(5000);
  }
  delay(100);
}

bool shouldLockScreen()
{
   return lockTimerStarted && (millis() - maxDistanceDetectionTime) / 1000 > getTimer();
}

bool shouldEnableLockTimer()
{
   int allowedDistance = percentMaxDistanceChangedAllowed / 100 * getDistance();
   
   return getTimer() > 1 && getDistance() > 10 && actualDistance - getDistance() > allowedDistance;
}

void writeStatusData()
{
    setDisplayText(1, "MinDistance:", String(getDistance())); 
    setDisplayText(1, "Timer:", String(getTimer()));
    setDisplayText(1, "ActualDistance:", String(actualDistance));
    int countDown = getTimer() - (millis() - maxDistanceDetectionTime) / 1000;
    setDisplayText(1, "Locking:", shouldEnableLockTimer() && countDown >= 0 ? ".." + String(countDown) : "no");
}

void initializeDisplay()
{
  GOFoled.init(0x3C); 
  GOFoled.clearDisplay();
  GOFoled.setCursor(0, 0);
}

void setDisplayText(byte fontSize, String label, String data)
{
  GOFoled.setTextSize(fontSize);
  GOFoled.println(label + ":" + data);
  //Serial.println(label + ":" + data);
}

void doDisplay()
{ 
  GOFoled.display();
}

void clearDisplay()
{
   GOFoled.clearDisplay();
   GOFoled.setCursor(0, 0);
}

int getActualDistance()
{
    int distanceSum = 0;
    for (byte i=0;i<10;i++) {
        distanceSum += ultrasonic.Ranging(CM);
    }

    return distanceSum / 10;
}

int getDistance()
{
  return map(analogRead(timerPot), 0, 1024, 0, 200);
}

int getTimer()
{
  return map(analogRead(distancePot), 0, 1024, 0, 20);
}

void lockScreen()
{
    Serial.println("pressing");
    Keyboard.press(KEY_LEFT_CTRL);
    delay(10);
    Keyboard.press(KEY_LEFT_ALT);
    delay(10);
    Keyboard.write('l');
    delay(10);
    Keyboard.releaseAll();  
}

