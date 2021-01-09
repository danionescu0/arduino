# Prerequisites

1. Arduino IDE installed on your system
Download link: https://www.arduino.cc/en/main/software

Boards pinouts, details:

**1** [NodeMCU](#NodeMCU)

**2** [Arduino Pro Mini](#Arduino-pro-mini)

**3** [Esp32-camera](#Esp32-camera)

**4** [Arduino Nano 33 BLE Sense](#Arduino-Nano-33-BLE-Sense)

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

**1** [TextMotorCommandsInterpretter](#TextMotorCommandsInterpretter) Given a string command it sets a robot speed and angle


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

**Components:**
* Arduino pro mini
* USB cable
* MPU6050 gyroscope
* neopixel led ring
* male-male, male-female cables
* breadboard
* 5V power supply for the led ring

## Neopixel ring gestures

Example usage of APDS9960 gesture sensor and a neopixel ring to animate a led rotation and change color.

Using up - down gestures the leds will change color

Using left - right gestures the leds will appeare move to left / right

YouTube video: https://youtu.be/EOPIJkmsgAo 

Full tutorial: https://www.instructables.com/id/Controlling-a-Neopixel-Led-Ring-With-a-Gesture-Sen


![NeopixelGestures](https://github.com/danionescu0/arduino/blob/master/projects/neopixel_ring_gestures/sketch_bb.png)
 
**Components:**
* arduino UNO
* USB cable
* APDS9960 gesture sensor
* neopixel led ring
* male-female
* breadboard
* 5 V power supply for the led ring (4 AA batteries case)

**The algorithm runs like this:**

- initialize the neopixel and gesture sensor libraries 

- create an array of led intensities called "ledStates". This array will contain 24 led intensities that 
are arranged in a descending manner from 150 to 2

- inside the main loop check if the interrupt pin has been modified if so it's time to change led's animation or color

- the "handleGesture()" function checks the last gesture and calls the function "toggleColor" for UP -DOWN 
gestures or set a global variable "ledDirection" for LEFT - RIGHT gestures

- the "toggleColor()" function simply changes a global variable named "colorSelection" with one of the values 0, 1, 2

- also inside the main loop function another function named "animateLeds();" is called. 
This function checks if 100 milliseconds passed, and if so it rotates the leds using "rotateLeds()" 
function and then redraws them

- the "rotateLeds()" basically will rotates the leds in any of the direction (forward or backward) using 
another array called "intermediateLedStates". For this first creates the new array and copies the old 
led intensities on the new positions (increment the position or decrement it). 

After that it overwrites the "ledStates" array with the "intermediateLedStates" so the process will
 continue after another 100 milliseconds.

## Keyboard exploit

In this project i'm using an arduino leonardo to simulate a possible USB attack using HID (humain interface device).

YouTube video: https://youtu.be/PsYTfWgX3eU

Full turorial: https://www.instructables.com/id/Arduino-Keyboard-Exploit-Demo-HID-and-Prevention/


**Important!: You can defend against this kind of attack by:**

​1. Disabling USB ports

- for windows you can check this tutorial:http://www.thewindowsclub.com/disable-enable-usb-windowunlock-pen-drive-at-office-or-school-computer

2. Whitelist USB devices:

- for windows: https://superuser.com/questions/1152012/block-unblock-usb-devices-except-whitelist

2. Lock your computer when your're not away

3. Don't login as root (require passwords for installing anything)

4. Keep your self up to date (automatic updates on)

The arduino leonardo can act like a keyboard and mouse, so the attack will be mounted like this:
 
**Components:**
* arduino leonardo
* usb cable
* micro usb card reader
* sd card
* push button
* male-female, female-female jumper cables

**How will the attack work:**
 
1. When the button is pressed, the leonardo will read the sd card using a sd card reader. 
A special file containing keys and key combination will be present on the card. 
The file name is "hack.txt".

The file can contain raw text, and it will passed to the keyboard just as it is. 

Also it can contain special commands like "Sleep::" and "Command::".
 
A line like: 
````
Sleep::200
````
means a sleep of 200 ms

A line like:
````
Command::KEY_LEFT_CTRL,KEY_LEFT_ALT,t
````
means left ctrl pressed, left alt pressed, t pressed and all released

You can check all special keys here: https://www.arduino.cc/en/Reference/KeyboardModifiers

2. Leonardo will read line by line, and interpret the commands and emulate the keys on the keyboard

The "hack.txt" contains a combination of keys that does the following (for UBUNTU linux):

a. opens a terminal

b. opens a python file for creation using vi

c. writes a python script inside that collects all text files inside of documents home folder
 and sends them over to a specified gmail address

d. runs the file in the background

e. deletes the file

f. closes the terminal

This whole thing runs in a few seconds and doesn't leave traces.

**To replicate the project:**

a. assemble the arduino leonardo: 
connect the button to digital pin 8, connect the card reader and the usb cable

b. edit the hack.txt file and modify the following lines with email and passwords:
````
smtp_user = 'sender_email_address'
smtp_pass = 'password'
to_address = 'receiver_email_address'
````
c. format the sd card using fat16 or fat32

e. copy the hack.txt file

e. ensure you have a test txt file in the Documents folder on your computer

f. plug the arduiono and press the button

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

The sketch controlls the angle and direction of the car using MPU6050 gyroscope

![Zenwheels remote controll](https://github.com/danionescu0/arduino/blob/master/projects/zen_wheels_remote/sketch_bb.jpg) 

Components: Arduino pro mini, bluetooth HC-05, MPU6050 gyroscope, breadboard, wires, power source

YouTube video: https://youtu.be/ih9J1sDsSLk

How to pair to a bluetooth slave module: http://phillipecantin.blogspot.com/2014/08/hc-05-bluetooth-link-with-zero-code.html

Steps:

- use your android phone to pair with the car and extract the car address

- set the bluetooth module in master mode, and set it to pair with the ZenWheels microcar

- assemble the arduino, bluetooth(pins 10 an 11) and MPU6050 (pins A4, A5) on the breadboard

- upload the code to the arduino

- power up the car (it could take a few minutes for the bluetooth module to connect)


## Camera web upload

Configure the sketch before the upload with your WIFI credentials, and the upload endpoint
````
const char* ssid = "wifi_network_name";
const char* password = "wifi_password";
const char* uploadUrl = "http://endpoint_url";
````

A good tutorial here: https://randomnerdtutorials.com/esp32-cam-video-streaming-face-recognition-arduino-ide/

## HASS geiger integration

YouTube video: https://youtu.be/auRfZ5q2SrY

This project will integrate a arduino geiger counter (one that supports serial logging of CPM) into HomeAssistant.
Full tutorial here: https://www.instructables.com/Home-Assistant-Geiger-Counter-Integration/

Home assistant is a great platform for home automation. If you don't know the platform you shoul check it out:) https://www.home-assistant.io/

![Diagram](https://github.com/danionescu0/arduino/blob/master/projects/HASSGeigerIntegration/diagram.jpg) 

There are many DYI arduino geiger counters:
- https://create.arduino.cc/projecthub/EDUcentrum/geiger-counter-with-arduino-uno-2cf621
- https://rhelectronics.net/store/radiation-detector-geiger-counter-diy-kit-second-edition.html
- https://www.tindie.com/products/rhgeiger/arduino-ide-geiger-counter-diy-kit-ver2-with-lcd/

I've got my self an aready assembled DYI product and i've purchased the tube separatly (SBM-20). The important thing here
is that your counter supports logging via the serial line. Mine does, it sends the CPM value periodically.

The integration with the Home assistant platform is done easily via MQTT. 
You will need your own server or a public one (default in the sketch).

The integation is done with NodeMCU board (https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/). 
The board is very cheap (around 4$ on ebay) and has WIFI and arduino IDE compatibility, you can programm it in C by
default.

Pinout:

- power Node Mcu through micro USB
- VIN goes to geiger counter + 
- GND goest to geiger counter -
- NodeMCU D7 (pin 13) goes to geiger TX 
- NodeMCU D8 (pin 15) goes to geiger RX
 

Minumum configuration on the sketch implies the WIFI SSID and password:

````
#define STASSID "" // Replace with your WIFI SSID
#define STAPSK  "" // Replace with your WIFI password
````

Also you can configure a sketch password (if you want to use OTA) or a different MQTT server:
````
#define SKETCHPASS "thepass" // Replace with your WIFI password
#define MQTT_SERVER "broker.hivemq.com" // OR replace with yout MQTT sderver
```` 
By default the sketch enables OTA (https://randomnerdtutorials.com/esp8266-ota-updates-with-arduino-ide-over-the-air/), 
if your're not familiar with the concept or if you don't want to use it  please comment this line
````
#define ENABLE_OTA // comment this line if you don't want OTA
````

If you dont' have "filed editor" in the meniu follow this guide: https://www.home-assistant.io/getting-started/configuration/

Next configure the configuration.yaml in your HASS, replace the broker if you have our own MQTT server:
````
mqtt:
  broker: broker.hivemq.com
  discovery: true
  discovery_prefix: ha
````
then under sensor add this:
````
sensor:
  #... your other sensors here
  ...
  - platform: mqtt
    name: "Radiation"
    state_topic: "ha/radiation"
    unit_of_measurement: 'mSv'
    unique_id: "radiation"
    value_template: "{{ value_json.radiation }}"    
````
Restart the Home assistant and that's it.

Troubleshooting:
- if you're using the free MQTT server from above and your're getting noise it means some one else it's using the topic.
Change the topic to a different name both in the arduino sketch and in HASS

###  Gesture capture

Uses the on-board IMU to start reading acceleration and gyroscope
data from on-board IMU and prints it to the Serial Monitor for one second
when the significant motion is detected, it also logs the data to a SD card

Saves each gesture data to a SD card in a separate file "1.csv", "2.csv" etc
based on example from https://github.com/arduino/ArduinoTensorFlowLiteTutorials
To record the next gesture swipe right with your hand in front of the board, it uses
the on board gesture detector to detect GESTURE_RIGHT and to record in the next csv file

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

*ArduinoBLE (version 1.1.3)

Board version in arduino IDE:

Arduino mbed-enabled board vs 1.1.6 (tested with this version, does not work with other versions)

PINOUT

*LED

D2     -  pin through a 220ohms resistor

GND    -  GND

Trained models:
digits_model.h -> uses gestures 0,1,2,3,4 (draw it in the air)

remote1.mode.h -> uses gestures cw,ccw,fwd (clockwise, counter clockwise, forward in the air)

# Libraries

### TextMotorCommandsInterpretter

Given a string command representing coordonates for X, Y axys, the library transforms
those on percentage of power for a two motor robot/car.

For the X (direction) between -50 and 50, and Y (power) between -50 and 50

And for example the command: 
"M:-25:16;" means (-25 left), and (16 power), it wil translate to
left motor 17% and right motor 32%, and direction forward

"M:44:19;" means (44 to the right) and (19 power) it will translate to:
left motor 38%, right motor 5% and direction forward

YouTube video: https://youtu.be/6FrEs4C9D-Y

Full tutorial here: https://www.instructables.com/id/Android-Controlled-Robot-Spy-Camera/

Usage example: 

````
#include <TextMotorCommandsInterpretter.h>
....
//declare instance, set X axys space from -50 to 50, Y axys space from -50 to 50
TextMotorCommandsInterpretter motorCommandsInterpretter(-50, 50, -50, 50);
...
//optional set command format
motorCommandsInterpretter.setCommandFormat('M', ':', ';');
// it will set command format with starting character 'M', ':' for internal delimiter, and ';' for command terminator.
// this is the default but you can change it to any characters
...
//use it to parse serial commands
motorCommandsInterpretter.analizeText("M:-25:16;");
float percentLeftMotor = motorCommandsInterpretter.getPercentLeft();
float percentRightMotor = motorCommandsInterpretter.getPercentRight();
boolean direction motorCommandsInterpretter.getDirection();

// percentLeftMotor will be 0.17
// percentRightMotor will be 0.32
// direction will be true
````

#Boards

## NodeMCU
![Nodemcu](https://github.com/danionescu0/arduino/blob/master/boards/nodemcu/nodemcu.jpg) 

For instalation in Arduino IDE follow the steps from /boards/nodemcu: https://github.com/danionescu0/arduino/tree/master/boards/nodemcu

## Arduino pro mini
![Arduino pro mini](https://github.com/danionescu0/arduino/blob/master/boards/arduinopromini.png)

## Esp32 camera
![esp32-camera](https://github.com/danionescu0/arduino/blob/master/boards/esp32-camera.jpg)

Instalation in Arduino IDE: https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-mac-and-linux-instructions/

## Arduino Nano 33 BLE Sense
![Arduino-nano-33-ble-sense](https://github.com/danionescu0/arduino/blob/master/boards/arduino-nano-33-ble-sense.png)

Overview: https://gilberttanner.com/blog/arduino-nano-33-ble-sense-overview