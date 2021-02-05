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

# Libraries

**1** [TextMotorCommandsInterpretter](https://github.com/danionescu0/arduino/blob/master/libraries/README.md) Given a string command it sets a robot speed and angle


## Computer auto lock

A computer lock mechanism that activates shortly after the user leaves the computer (measures distance)

Full tutorial here: https://www.instructables.com/id/Auto-Lock-Computer-System/

YouTube video: https://youtu.be/b7RvvMIwWFs

![ComputerAutoLock](https://github.com/danionescu0/arduino/blob/master/projects/computer_auto_lock/sketch_bb.jpg)

## Neopixel ring gyroscope

Tilting the breadboard with the neopixel ring and a MPU6050 gyroscope will make led light up in the tilt direction.

YouTube video: https://youtu.be/MFQ2PecTw8g

Full tutorial here: https://www.instructables.com/id/Gyroscope-Fun-With-Neopixel-Ring/

![Gyroscope](https://github.com/danionescu0/arduino/blob/master/projects/neopixel_ring_gyroscope/sketch_bb.png)

## Neopixel ring gestures

Example usage of APDS9960 gesture sensor and a neopixel ring to animate a led rotation and change color.

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

This project will control led's using gyroscope tilt.

Full tutorial: https://www.instructables.com/id/Giroscope-led-controll-with-Arduino/

YouTube video: https://youtu.be/lYH1H_nWLz4


## Zenwheels remote controll

The sketch controlls the angle and direction of the Zenwheels toy car using MPU6050 gyroscope

![Zenwheels remote controll](https://github.com/danionescu0/arduino/blob/master/projects/zen_wheels_remote/sketch_bb.jpg) 

YouTube video: https://youtu.be/ih9J1sDsSLk


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

### Gesture classifier

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
