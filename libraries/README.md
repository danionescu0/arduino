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
