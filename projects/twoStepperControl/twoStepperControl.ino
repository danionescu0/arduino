#include <TFT.h>

#include <Servo.h>

Servo servo1; 
Servo servo2;   

void setup() {
  servo1.attach(9);
  servo2.attach(6);
}

void loop() {
   goBackAndForward(servo1, 80, 100, 6, 3);
   delay(1000);
   goBackAndForward(servo2, 80, 100, 6, 3);
   delay(2000);
}

void goBackAndForward(Servo servo, byte start, byte end, byte wait, byte times) {
    for (int i=0; i < times; i++) {
        int pos = 0;  
        for (pos = start; pos <= end; pos += 1) { 
          servo.write(pos);
          delay(wait);                  
        }
        for (pos = end; pos >= start; pos -= 1) { 
          servo.write(pos);              
          delay(wait);                      
        }
    }
}
