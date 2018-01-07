/* 
 Libraries used:

 MegaServo: https://playground.arduino.cc/Code/MegaServo
 
 Example bluetoth serial controll commands:
    Left: 80
    Forward: 90
    Right:100
    Left 45 degreeds:25
    Right 45 degreeds:155
    Start / Stop: s
    Up / Down: d

  Configuration:

  Configure actuator motors[] angles of your own robot
  
*/
#include <MegaServo.h>
#include <VirtualWire.h>

MegaServo Servos[18];

int i, j, newPos, changeAngleWith;
int delayBtwMooves;
const int defaultSpeedFactor = 10; // multiplied by a factor of 5, representing the delay after each group leg movements
const char inputToggleMotion = 's';
const char inputToggleElevation = 'd';
int speed, direction;
boolean finishedCurrentMove = false;
boolean hasMotion = false;
byte isUp = 1;

//definitions for serial comm ex bluetooth
char buffer[] = {' ',' ',' ',' '};

struct remoteC
  {
      uint8_t speedFactor;
      uint8_t direction;
      uint8_t special;    
  }; //remote controll return structure

remoteC controller;
remoteC getCommandsBT();

typedef struct
  {
      byte id;    
      byte pin;
      int dir; // 1 representing forward in space, -1 reprezenting backwards
      byte middlePos;
      char elevation; // Low, Medium, Top
      char side; // Left, Right
  }  actuator;

typedef struct
  {
    byte id;
    int dir;
    byte angle;
  } 
  walkStep;
  
typedef struct
  {
      MegaServo srv;
      int oldPos;
      int newPos;
  }
  servoStep;
  
servoStep servoSequence[18]; // stores steps for smooth mooving function
int servoSequenceNr = 0;

// values for adjusting calibrated default middle positions
// M = medium, T = top
byte adjustM = 5;
byte adjustT = 0;
byte adjustB = -14;

//motors mappings
// to increase angle increase middlePos with positive value multiplied by dir
actuator motors[] =
            {
                {1,  33,  1, 70 + adjustT, 't', 'l'},
                {2,  40, -1, 95  - adjustT, 't', 'l'},
                {3,  41, -1, 27  - adjustT, 't', 'l'},
                {4,  42,  1, 137 + adjustT, 't', 'r'},
                {5,  43,  1, 137 + adjustT, 't', 'r'},
                {6,  44,  1, 115 + adjustT, 't', 'r'},
                {7,  34,  1, 40  + adjustM, 'm', 'l'},
                {8,  35,  1, 14  + adjustM, 'm', 'l'},
                {9,   3,  1, 18  + adjustM, 'm', 'l'},
                {10,  5, -1, 55  - adjustM, 'm', 'r'},
                {11,  7, -1, 65  - adjustM, 'm', 'r'},
                {12,  6, -1, 55  - adjustM, 'm', 'r'},
                {13, 38, -1, 16  - adjustB, 'l', 'l'},
                {14, 37, -1, 10  - adjustB, 'l', 'l'},
                {15, 53, -1, 8   - adjustB, 'l', 'l'},
                {16, 23, -1, 12  - adjustB, 'l', 'r'},
                {17, 25,  1, 107 + adjustB, 'l', 'r'},
                {18, 26,  1, 115 + adjustB, 'l', 'r'}             
            };

int walkStepCounter = 0; //where does the step left off
//walking gait mapping            
walkStep steps[][6] = 
            {
              {
                {8, 1, 8},
                {10, 1, 8},
                {12, 1, 8}                
              },
              {
                {2, 1, 8},
                {4, 1, 8},
                {6, 1, 8},
                {1, -1, 8},
                {3, -1, 8},
                {5, -1, 8}                
              },
              {
                {8, -1, 8},
                {10, -1, 8},
                {12, -1, 8}                   
              },
              {
                {7, 1, 8},
                {9, 1, 8},
                {11, 1, 8}                
              },
              {
                {2, -1, 8},
                {4, -1, 8},
                {6, -1, 8},
                {1, 1, 8},
                {3, 1, 8},
                {5, 1, 8}                
              },      
              {
                {7, -1, 8},
                {9, -1, 8},
                {11, -1, 8}                
              }              
            };  

walkStep sitSteps[12] =
            {
              {7, 1, 75},
              {8, 1, 75},
              {9, 1, 75},
              {10, 1, 75},
              {11, 1, 75},
              {12, 1, 75},
              {13, -1, 80},
              {14, -1, 80},
              {15, -1, 80},
              {16, -1, 80},
              {17, -1, 80},
              {18, -1, 80}
            };

void setup() 
{ 
    Serial.begin(57600);
  
    for (i=0; i<18; i++) {    
        Servos[i].attach(motors[i].pin, 800, 1800);
    }
    initCommunication();
    struct remoteC controller = {70, 90, ' '};
    resetDefaults();
    Serial.println("Finish initialization");
} 
 
void loop() 
{ 
    controller = getCommandsBT(); 
    if (controller.special == inputToggleMotion) {
        resetDefaults();      
        controller.special = ' ';
        hasMotion = !hasMotion;
    }
    if (controller.special == inputToggleElevation) { 
        isUp = isUp == 1 ? 0 : 1; 
        controller.special = ' '; 
        if (isUp) {      
            resetDefaults();  
        } else {                    
            sitDown(40, 25);
        }
    }
    if (hasMotion) {
        finishedCurrentMove = walkForward(controller.direction, controller.speedFactor * 5);
    }
} 

void sitDown(int smoothing, int delayBtwMoves)
{
    int i;
    for (i=0; i<12; i++) {
        walkStep stepX = sitSteps[i];
        actuator currActuator = motors[stepX.id - 1];
        Servos[stepX.id - 1].read();
        int newAngle = Servos[stepX.id - 1].read() + stepX.dir * stepX.angle * currActuator.dir;
        addSmoothMove(Servos[stepX.id - 1], newAngle, Servos[stepX.id - 1].read()); 
    }  
    smoothMove(smoothing, delayBtwMoves);
    delay(200);
}

/**
* Advance hexapod position by executing steps stored in walkStep variable
*
* @param direction :as follows: 90 forwars moovement, 0 full left, 180 complete right
* @param delayBtwMooves : after each set of walkStep a pause of ms is required 
* @return boolean finishedMoovement
*/
boolean walkForward(int direction, int delayBtwMooves)
{  
    Serial.print("step=");Serial.print(walkStepCounter);Serial.print("; ");
    int lastAngle = 0;
    for (j=0; j<6; j++) {
        walkStep step = steps[walkStepCounter][j];
        if (step.id == 0) {
            continue; // skip where array index is not defined
        }
        actuator currActuator = motors[step.id - 1];
        lastAngle = step.angle;  // we need last angle to calculate how smooth is the moove (we presume last angle is same for all motors in gorup)          
        changeAngleWith = calculateAngleFromDirection(direction, step.angle, currActuator.side, currActuator.elevation);
        int newAngle = currActuator.middlePos + currActuator.dir * step.dir * changeAngleWith;
        Serial.print(currActuator.pin);Serial.print("->");Serial.print(newAngle);Serial.print(" "); 
        addSmoothMove(Servos[step.id - 1], newAngle, Servos[step.id - 1].read());        
    }
    Serial.println(" ");Serial.println();
    smoothMove(lastAngle * 2, 8);
    delay(delayBtwMooves);
    walkStepCounter++;
    if (walkStepCounter >= 6) {
        walkStepCounter = 0;
        return true;
    }
    
    return false;    
}

int calculateAngleFromDirection(int direction, int defaultAngle, char actuatorSide, char actuatorElevation)
{
    if (actuatorElevation != 't') {
        return defaultAngle; // calculate angle for top actuators, they are the only one that make the turn
    }
    if ( (actuatorSide == 'l' and direction >= 90) || (actuatorSide == 'r' and direction <= 90) ) {
      return defaultAngle; //skips computation if the actuator is located on the other side
    }
    int computedAngle = 0;
    if (direction > 90 && direction <= 145) {
        computedAngle = map(direction, 145, 90, 0, defaultAngle);
    } else if (direction < 90 && direction >= 45) {
        computedAngle = map(direction, 45, 90, 0, defaultAngle);
    } else if (direction < 45 && direction >= 0) {
        computedAngle = map(direction, 45, 0, 0, defaultAngle) * -1;
    } else if (direction > 145 && direction <= 180) {
        computedAngle = map(direction, 145, 180, 0, defaultAngle) * -1;
    }  
    
    return computedAngle;
}

void resetDefaults() 
{
    int i, j, k, angle = 0;
    for (i=0; i<18; i++) {    
         for (j=0; j<6; j++) {
           for (k=0; k<6; k++) {
               if (steps[j][k].id == motors[i].id) {
                   angle = steps[j][k].angle;
                   break;
               }             
           }    
           if (angle != 0) {
               break;
           }       
         }
         addSmoothMove(Servos[i], motors[i].middlePos - angle * motors[i].dir, Servos[i].read());
    }  
    smoothMove(25, 30);
}

void addSmoothMove(MegaServo servo, int newPosition, int oldPosition)
{
    servoSequence[servoSequenceNr].srv = servo;
    servoSequence[servoSequenceNr].newPos = newPosition;
    servoSequence[servoSequenceNr].oldPos = oldPosition;
    servoSequenceNr++;
}

void smoothMove(int nrSteps, int delayBtwMooves)
{
    int i, j;
    for (i = 1; i <= nrSteps; i++) {
        for (j = 0; j < servoSequenceNr; j++) {
            float stepSize = (float) abs(servoSequence[j].newPos - servoSequence[j].oldPos) / (float) nrSteps;
            float newPosition = stepSize * i;
            if (servoSequence[j].newPos > servoSequence[j].oldPos) {
                newPosition = servoSequence[j].oldPos + newPosition;
                if (newPosition > servoSequence[j].newPos) {
                    newPosition = servoSequence[j].newPos;
                }
            } else {
                newPosition = servoSequence[j].oldPos - newPosition;          
                if (newPosition < servoSequence[j].newPos) {
                    newPosition = servoSequence[j].newPos;
                }                
            }          
            //Serial.print(j);Serial.print("->");Serial.print(servoSequence[j].oldPos);Serial.print(",");
            //Serial.print(servoSequence[j].newPos);Serial.print(",");Serial.print(newPosition);Serial.print("; ");
            servoSequence[j].srv.write(newPosition);
        }
        //Serial.println();
        delay(delayBtwMooves);
    }
    servoSequenceNr = 0;
}

void initCommunication()
{
    Serial1.begin(9600);
    Serial1.setTimeout(70);
}  

struct remoteC getCommandsBT()
{ 
     if (Serial1.available() == 0) {  
         return controller;
     }
     Serial.println("Got message");
     Serial1.readBytesUntil('\n', buffer, 4);
     if (buffer[0] == inputToggleMotion || buffer[0] == inputToggleElevation) {
         Serial.print("Received special function: ");Serial.println(buffer[0]);
         controller.special = buffer[0];
         controller.speedFactor = defaultSpeedFactor;
         controller.direction = 80;
         
         return controller; 
     }   
     uint8_t incomingValue = atoi(buffer);      
     controller.speedFactor = defaultSpeedFactor;
     controller.direction = incomingValue; 
     controller.special = ' ';
     Serial.print(controller.speedFactor);Serial.print("____");Serial.println(controller.direction);          
     for (i = 0; i<= 4; i++) {
         buffer[i] = ' ';
     }       
  
    return controller;
}
