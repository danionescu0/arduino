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

# Projects

## neopixel_ring_gyroscope

Full turorial here: https://www.instructables.com/id/Gyroscope-Fun-With-Neopixel-Ring/

![ifttt.png](https://github.com/danionescu0/arduino/blob/master/projects/neopixel_ring_gyroscope/sketch_bb.png)

## keyboard_exploit

In this project i'm using an arduino leonardo to simulate a possible USB attack using HID 
(humain interface device).


**Important!: You can defend against this kind of attack by:**

* disabling USB ports

* locking your computer when your're away

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
A special file containg keys and key combination will be present on the card. 
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

My "hack.py" contains a combination of keys that does the following (for UBUNTU linux):

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


