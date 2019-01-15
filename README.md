# Projects


## computer_auto_lock


A computer lock mechanism that activates shortly after the user leaves the computer (measures distance)


![ifttt.png](https://github.com/danionescu0/arduino/blob/master/projects/computer_auto_lock/sketch_bb.png)

## neopixel_ring_gyroscope

YouTube video: https://youtu.be/MFQ2PecTw8g

Full tutorial here: https://www.instructables.com/id/Gyroscope-Fun-With-Neopixel-Ring/

Tilting the breadboard with the neopixel ring and a MPU6050 gyroscope will make led light up in the tilt direction.


![ifttt.png](https://github.com/danionescu0/arduino/blob/master/projects/neopixel_ring_gyroscope/sketch_bb.png)

**Components:**
* arduino pro mini
* USB cable
* MPU6050 gyroscope
* neopixel led ring
* male-male, male-female cables
* breadboard
* 5 V power supply for the led ring

## neopixel_ring_gestures

YouTube video: https://youtu.be/EOPIJkmsgAo 

Full tutorial: https://www.instructables.com/id/Controlling-a-Neopixel-Led-Ring-With-a-Gesture-Sen

Example usage of APDS9960 gesture sensor and a neopixel ring to animate a led rotation and change color.

Using up - down gestures the leds will change color

Using left - right gestures the leds will appeare move to left / right

![ifttt.png](https://github.com/danionescu0/arduino/blob/master/projects/neopixel_ring_gestures/sketch_bb.png)
 
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

## keyboard_exploit

YouTube video: https://youtu.be/PsYTfWgX3eU

Full turorial: https://www.instructables.com/id/Arduino-Keyboard-Exploit-Demo-HID-and-Prevention/

In this project i'm using an arduino leonardo to simulate a possible USB attack using HID 
(humain interface device).


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