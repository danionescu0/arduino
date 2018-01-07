#include <CustomStepper.h>
 
CustomStepper leftMotor(6, 7, 8, 9, (byte[]) {8, B1000, B1100, B0100, B0110, B0010, B0011, B0001, B1001}, 4075.7728395, 12, CCW);
CustomStepper rightMotor(2, 3, 4, 5, (byte[]) {8, B1000, B1100, B0100, B0110, B0010, B0011, B0001, B1001}, 4075.7728395, 12, CW);

//#define DEBUG true;
float Kp=6, Ki=0, Kd=0;
float error=0, P=0, I=0, D=0, PID_value=0;
float previous_error=0, previous_I=0;
int sensor[5] = {0, 0, 0, 0, 0};
const int maxSpeed = 20;
long lastTransmitted;
int lastDefinedError = 0;
long minTransmittingInterval = 1000;
long maxTransmittingInterval = 1200;

struct errorMap 
{
   int left1;
   int left2;
   int middle;
   int right1;
   int right2;
   int error;
};
errorMap sensorErrorMap[] = 
{
    {1, 1, 1, 1, 0, 4},
    {1, 1, 1, 0, 0, 3},
    {1, 1, 1, 0, 1, 2},
    {1, 1, 0, 0, 1, 1},
    {1, 1, 0, 1, 1, 0},
    {1, 0, 0, 1, 1, -1},
    {1, 0, 1, 1, 1, -2},
    {0, 0, 1, 1, 1, -3},
    {0, 1, 1, 1, 1, -4},    
};

void read_sensor_values(void);
void calculate_pid(void);
void motor_control(void);

void setup()
{
    Serial.begin(9600); 
    leftMotor.setSPR(4075.7728395);
    rightMotor.setSPR(4075.7728395);    
    leftMotor.rotate();
    rightMotor.rotate();    
    lastTransmitted = millis();   
}

void loop()
{
    read_sensor_values();
    calculate_pid();
    motor_control();    
    leftMotor.run();
    rightMotor.run(); 
}

void read_sensor_values()
{
    sensor[0] = digitalRead(A0);
    sensor[1] = digitalRead(A1);
    sensor[2] = digitalRead(A2);
    sensor[3] = digitalRead(A3);
    sensor[4] = digitalRead(A4);
    for (byte i = 0; i < 9; i++) {     
        if (sensor[0] == sensorErrorMap[i].left1 && sensor[1] == sensorErrorMap[i].left2 && 
            sensor[2] == sensorErrorMap[i].middle && sensor[3] == sensorErrorMap[i].right1 && 
            sensor[4] == sensorErrorMap[i].right2) {
            error = sensorErrorMap[i].error;
        }
        if (sensor[0] + sensor[1] + sensor[2] + sensor[3] + sensor[4] == 5) {
            error = lastDefinedError;
        } else {
            lastDefinedError = error;              
        }
    }
    #if defined(DEBUG)
      print("Error=", false);print(String(error), true);
    #endif
}

void calculate_pid()
{
    P = error;
    I = I + previous_error;
    D = error - previous_error;    
    PID_value = (Kp * P) + (Ki * I) + (Kd * D);    
    previous_I = I;
    previous_error = error;
    #if defined(DEBUG)
        print("PID=", false);print(String(PID_value), true);
        delay(500);
    #endif
}

void setMotorSpeed(int left, int right)
{
    leftMotor.setRPM(left);
    rightMotor.setRPM(right);  
}

void motor_control()
{
    int leftMotorSpeed = maxSpeed + PID_value;
    int rightMotorSpeed = maxSpeed - PID_value;
   
    leftMotorSpeed = constrain(leftMotorSpeed, 0, maxSpeed);
    rightMotorSpeed = constrain(rightMotorSpeed, 0, maxSpeed);
    #if defined(DEBUG)
        print("left=", false);print(String(leftMotorSpeed), true);
        print("right=", false);print(String(rightMotorSpeed), true);
    #endif
    setMotorSpeed(leftMotorSpeed, rightMotorSpeed);
}

void print(String stuff, boolean newline) 
{
    if (millis() - lastTransmitted < minTransmittingInterval) {
        return;        
    }  
    if (millis() - lastTransmitted > maxTransmittingInterval) {
        lastTransmitted = millis();
    }
    if (newline) {
        Serial.println(stuff);
    } else {
        Serial.print(stuff);   
    }  

    delay(3);
}

