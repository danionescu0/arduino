# Prerequisites

Arduino IDE installed on your computer: Download link: https://www.arduino.cc/en/main/software

About boards and pinouts:

* [NodeMCU](https://github.com/danionescu0/arduino/blob/master/boards/README.md)
* [Arduino Pro Mini](https://github.com/danionescu0/arduino/blob/master/boards/README.md)
* [Esp32-camera](https://github.com/danionescu0/arduino/blob/master/boards/README.md)
* [Arduino Nano 33 BLE Sense](https://github.com/danionescu0/arduino/blob/master/boards/README.md)

# Projects

**1** [Computer auto lock system](#Computer-auto-lock) A computer lock mechanism that activates shortly after the user leaves the computer

**2** [Neopixel ring gyroscope](#Neopixel-ring-gyroscope) Tilting the breadboard with the neopixel ring and a MPU6050 gyroscope will make led light up in the tilt directionr

**3** [Neopixel ring gestures](#Neopixel-ring-gestures) Example usage of APDS9960 gesture sensor and a neopixel ring to animate a led rotation and change color

**4** [Keyboard exploit](#Keyboard-exploit) In this project i'm using an arduino leonardo to simulate a possible USB attack using HID (humain interface device)

**5** [Line follower](#Line-follower) A plain line follower with two stepper motors

**6** [Gyroscope led controll](#Gyroscope-led-controll) This project will control led's using gyroscope tilt

**7** [ZenWheelsRemote](#Zenwheels-remote-controll) This project will controll ZenWheels microcar with a gyroscope (work in progress)

**8** [CameraWebUpload](#Camera-web-upload) Uses the ESP32-cam to take pictures regularry and upload them using an api 

**9** [HASSGeigerIntegration](#HASS-geiger-integration) show arduino geiger counter readings on Home assistant (https://www.home-assistant.io/)

**10** [GestureClassifier](#Gesture-classifier) capture, train & predict gestures using Arduino nano BLE sensnse (work in progress)

**11** [Hexapod](#Hexapod) a project that enebles a hexapod to walk

# Libraries

**1** [TextMotorCommandsInterpretter](https://github.com/danionescu0/arduino/blob/master/libraries/README.md) Given a string command it sets a robot speed and angle


## Computer auto lock

Dive into the realm of computer screen security with our intriguing tutorial. 
Explore the vulnerabilities of default screen lock timeouts in operating systems, where a minute of inactivity can expose your computer to unauthorized access. 
Discover the solution to this dilemma as we tackle the challenge of creating a device that automatically locks your computer when you're away. 
Join us on this exciting journey to enhance your computer's security and protect your valuable data.

Full tutorial here: https://www.instructables.com/id/Auto-Lock-Computer-System/

YouTube video: https://youtu.be/b7RvvMIwWFs

![ComputerAutoLock](https://github.com/danionescu0/arduino/blob/master/projects/computer_auto_lock/sketch_bb.jpg)

## Neopixel ring gyroscope

Discover the magic of the MPU6050 gyroscope and Neopixel ring in our tutorial. Create a captivating LED display that responds to angle inclination using Arduino. 
Follow the step-by-step instructions to build this mesmerizing project on a breadboard and explore the impressive capabilities of these components. 
Unleash your creativity and embark on an illuminating journey of learning and fun.

YouTube video: https://youtu.be/MFQ2PecTw8g

Full tutorial here: https://www.instructables.com/id/Gyroscope-Fun-With-Neopixel-Ring/

![Gyroscope](https://github.com/danionescu0/arduino/blob/master/projects/neopixel_ring_gyroscope/sketch_bb.png)

## Neopixel ring gestures

Immerse yourself in the world of gesture-controlled electronics with our comprehensive tutorial. Explore the enchanting combination of the APDS-9960 gesture sensor and a Neopixel ring, seamlessly integrated with an Arduino UNO.

Witness the magic unfold as your end product responds to left-right gestures with captivating LED movements and up-down gestures by dynamically changing LED colors. 
Discover the part list, connect the components, and unravel the code step-by-step, gaining invaluable insights into its inner workings. 
Unleash your creativity and embark on this mesmerizing journey today!

Using up - down gestures the leds will change color

Using left - right gestures the leds will appeare move to left / right

YouTube video: https://youtu.be/EOPIJkmsgAo 

Full tutorial: https://www.instructables.com/id/Controlling-a-Neopixel-Led-Ring-With-a-Gesture-Sen

![NeopixelGestures](https://github.com/danionescu0/arduino/blob/master/projects/neopixel_ring_gestures/sketch_bb.png)
 

## Keyboard exploit

In this project i'm using an arduino leonardo to simulate a possible USB attack using HID (humain interface device).

YouTube video: https://youtu.be/PsYTfWgX3eU

Full turorial: https://www.instructables.com/id/Arduino-Keyboard-Exploit-Demo-HID-and-Prevention/

**Important!: You can defend against this kind of attack by:**

1. Disabling USB ports

- for windows you can check this tutorial:http://www.thewindowsclub.com/disable-enable-usb-windowunlock-pen-drive-at-office-or-school-computer

2. Whitelist USB devices:

- for windows: https://superuser.com/questions/1152012/block-unblock-usb-devices-except-whitelist

2. Lock your computer when your're not away

3. Don't login as root (require passwords for installing anything)

4. Keep your self up to date (automatic updates on)


## Line follower

A plain line follower with two stepper motors.

This sketch uses PID algrithm (https://blog.opticontrols.com/archives/344)

![line_follower](https://github.com/danionescu0/arduino/blob/master/projects/line_follower/sketch_bb.jpg)

YouTube video: https://youtu.be/oPWKSHsfMBM


## Gyroscope led controll
Experience a mesmerizing play of light and motion with our DIY project. 
Learn to construct a tilt-controlled dimmer using an Arduino Uno, gyroscope, and four LEDs arranged in a "+" shape on a breadboard. 
As you tilt, the LEDs intensify, creating a captivating visual display. 
Let your creativity shine with this simple yet enchanting electronics endeavor.

Full tutorial: https://www.instructables.com/id/Giroscope-led-controll-with-Arduino/

YouTube video: https://youtu.be/lYH1H_nWLz4


## Zenwheels remote controll
Reverse engineer the ZenWheels microcar Android app to decode its communication protocol. 
This project guides you in building a custom remote control using Arduino and a gyroscope. 
Unleash your technical skills and transform your 5 cm toy car experience. 
Full tutorial: https://www.instructables.com/member/danionescu/instructables/

YouTube video: https://youtu.be/ih9J1sDsSLk

The sketch controlls the angle and direction of the Zenwheels toy car using MPU6050 gyroscope

![Zenwheels remote controll](https://github.com/danionescu0/arduino/blob/master/projects/zen_wheels_remote/sketch_bb.jpg) 



## Camera web upload

Configure the sketch before the upload with your WIFI credentials, and the upload endpoint
````
const char* ssid = "wifi_network_name";
const char* password = "wifi_password";
const char* uploadUrl = "http://endpoint_url";
````

A good tutorial about esp32 video camera here: https://randomnerdtutorials.com/esp32-cam-video-streaming-face-recognition-arduino-ide/


## HASS geiger integration

YouTube video: https://youtu.be/auRfZ5q2SrY

This project will integrate a arduino geiger counter (one that supports serial logging of CPM) into HomeAssistant.
Full tutorial here: https://www.instructables.com/Home-Assistant-Geiger-Counter-Integration/

Home assistant is a great platform for home automation. If you don't know the platform you shoul check it out:) https://www.home-assistant.io/

## Gesture classifier

Detects gestures that wore previously trained with gesture_capture.ino After an accelerationThreshold is reached, 
the sketch begin recording numSamples (119) and then feeds in into TensorFlowLite to predict the gesture.

###  Gesture capture

Uses the on-board IMU to start reading acceleration and gyroscope
data from on-board IMU and prints it to the Serial Monitor for one second
when the significant motion is detected, it also logs the data to a SD card

Saves each gesture data to a SD card in a separate file "1.csv", "2.csv" etc
based on example from https://github.com/arduino/ArduinoTensorFlowLiteTutorials
To record the next gesture swipe right with your hand in front of the board, it uses
the on board gesture detector to detect GESTURE_RIGHT and to record in the next csv file

PINOUT

CARD READER 5V variant

* MOSI   -  pin 11  
* MISO   -  pin 12
* CLK    -  pin 13
* CS     -  pin 4
* GND    -  GND
* VCC    -  VIN
 
 LED
 
* D2     -  pin through a 220ohms resistor
* GND    -  GND

### Gesture classifier

Detects gestures that wore previously trained with gesture_capture.ino
After an accelerationThreshold is reached, the sketch begin recording numSamples (119)
and then feeds in into TensorFlowLite to predict the gesture.

The LED is used to signal that the sketch is recording a gesture
If you want to use your own trained data, replace #include "digits_model.h" with your own, then replace const char* GESTURES[] 
with your own trained gestures
                          
Arduino IDE vs: 1.8.12

Libraries: 

* Arduino_LSM9DS1 (version 1.0.0)

* TensorFlowLite (version 2.1.0 ALPHA precompiled)

* ArduinoBLE (version 1.1.3)

Board version in arduino IDE:

Arduino mbed-enabled board vs 1.1.6 (tested with this version, does not work with other versions)

PINOUT

LED

* D2   -  pin through a 220ohms resistor

* GND    -  GND

Trained models:

digits_model.h -> uses gestures 0,1,2,3,4 (draw it in the air)

remote1.mode.h -> uses gestures cw,ccw,fwd (clockwise, counter clockwise, forward in the air)

## Hexapod

### About:
Overall, the code provides a framework for controlling a hexapod robot using servo motors and receiving commands wirelessly through a Bluetooth module. It defines different functions for smooth movement, walking gait, initialization, and communication with a remote controller.

### Components
* Arduino Mega 2560
* Arduino Mega servo shield
* 18 servo motors (5V), eg: MG995
* hexapod skeleton (kit) which contains laser cut parts
* serial bluetooth module HC-05
* battery (i used LiPo 2S 3000mAH)
* high power step down regulator ( output >= 8A )

### Commanding the hexapod via bluetoogh
Commanding the Hexapod via Bluetooth

To control the hexapod robot wirelessly, you can use a serial remote control application on your Android or iOS device. Follow the steps below to establish a connection and program the buttons in the app to send serial commands to the hexapod:

Pairing with the Bluetooth Module:

* Make sure the Bluetooth module is properly connected to the hexapod robot.
* On your mobile device, go to the Bluetooth settings and search for available devices.
* Pair your mobile device with the Bluetooth module of the hexapod robot.

Programming Button Commands:

* Open the serial remote control application on your mobile device.
* In the app settings or configuration section, locate the button programming feature.
* Assign the following commands to the corresponding buttons:

1) Left: Send the command "80" to the hexapod.

2) Forward: Send the command "90" to the hexapod.

3) Right: Send the command "100" to the hexapod.

4) Left 45 degrees: Send the command "25" to the hexapod.

5) Right 45 degrees: Send the command "155" to the hexapod.

6) Start/Stop: Send the command "s" to the hexapod.

7) Up/Down: Send the command "d" to the hexapod.

Controlling the Hexapod:

* To initiate the hexapod's movement, send the "s" command. By default, it will start walking forward.
* To make the hexapod walk slightly to the left, send the command "80". For a 45-degree left turn, send "25".
* To stop the hexapod's movement, send the command "s". To start again, send "s".
* To make the hexapod walk slightly to the right, send the command "100". For a 45-degree right turn, send "155".
* To make the hexapod sit down or stand up, send the command "d". This will toggle the elevation between sitting and standing positions.

By programming the buttons in the serial remote control application, you can conveniently command the hexapod robot's movements and adjust its elevation wirelessly through Bluetooth.

### Libraries:

MegaServo: This library allows the control of multiple servo motors simultaneously.
VirtualWire: This library provides support for sending and receiving messages wirelessly using RF modules.

### Global Variables and Constants:

Various global variables and constants are defined, including variables for servo positions, speed factors, delay values, control commands, and flags.
Structures:

The code defines several structures to represent different aspects of the hexapod robot, such as actuators, walking steps, servo steps, and remote control commands.
Actuator Mapping:

An array named "motors" is defined to store the mapping of actuators. Each element of the array represents an actuator and contains information such as the actuator ID, pin number, direction, middle position, elevation, and side.
Walking Steps:

A 2D array named "steps" represents the walking gait of the hexapod. It contains sequences of walk steps for different directions and leg movements.

### Functions

Initialization (setup) Function: The setup function initializes the serial communication and servo motors. It also initializes the communication for receiving commands from a Bluetooth device and sets the initial values for the remote controller.
Main Loop (loop) Function:

The loop function continuously checks for incoming commands from the Bluetooth device.

If the received command is a special function command (e.g., start/stop, up/down), the corresponding action is performed.

If the hexapod is allowed to move, it executes the walkForward function with the given direction and speed factor.

The walkForward function processes the walking gait by calculating the required angles for each actuator based on the direction.

The resetDefaults function resets the hexapod to its default position by smoothly moving each servo motor to its initial angle.

The smoothMove and addSmoothMove functions are used for smoothly transitioning between different servo positions.

The sitDown function is used to make the hexapod sit down by adjusting the angles of the servo motors.

The calculateAngleFromDirection function calculates the angle for a specific actuator based on the direction and actuator configuration.
Communication Functions:

The initCommunication function initializes the communication for receiving commands from a Bluetooth device.

The getCommandsBT function reads and processes incoming commands from the Bluetooth device.


