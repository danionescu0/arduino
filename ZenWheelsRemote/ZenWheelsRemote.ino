#include <SoftwareSerial.h>

SoftwareSerial BTserial(10, 11);

const byte commandStering = 129;
const byte commandSpeed = 130;

void setup() {
  Serial.begin(9600);
  BTserial.begin(38400);
}

void loop() {
  Serial.println("transmitting");
  for (byte i = 0;i<=90;i+=5) {
    move(i, 7);
    delay(100);
  } 
  for (byte i= 91;i<=180;i+=5) {
    move(i, 7);
    delay(100);
  }  
  delay(2000);
}

void move(byte angle, byte speed) {
    byte resultAngle;
    if (angle <= 90) {
      resultAngle = map(angle, 1, 90, 1, 63);    
      byte val = map(angle, 1, 90, 1, 63);  
      Serial.println(val);
    } else if (angle > 90) {
      resultAngle = map(angle, 91, 180, 64, 126);
    }
    Serial.print(angle);Serial.print("-");Serial.println(resultAngle);
    BTserial.write(commandStering);    
    BTserial.write(resultAngle); 
    BTserial.write(commandSpeed);
    BTserial.write(speed); 
}

