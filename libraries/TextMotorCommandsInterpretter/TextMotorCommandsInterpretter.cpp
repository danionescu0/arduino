#include <TextMotorCommandsInterpretter.h>

TextMotorCommandsInterpretter::TextMotorCommandsInterpretter(int minX, int maxX, int minY, int maxY)
{
	_minX = minX;
	_maxX = maxX;
	_minY = minY;
	_maxY = maxY;
	_debug = false;
}

void TextMotorCommandsInterpretter::analizeText(String message)
{
	_message = message;
	_power = _getMotorPower();
	int direction = _getMotorDirection();
    float leftMotor, rightMotor;
    int middleDirection = round((_maxX + _minX) / 2);
    if (direction < 0) {
        leftMotor = map(direction, middleDirection, _minX, 100, 0);
        rightMotor = 100;
    } else {
        leftMotor = 100;
        rightMotor = map(direction, middleDirection, _maxX, 100, 0);
    }
    int middlePower = round((_maxY + _minY) / 2);
    float realPower = map(abs(_power), middlePower, _maxY, 0, 100);
    if (_debug) {
		Serial.print("LMotor=");Serial.println(leftMotor);
		Serial.print("RMotor=");Serial.println(rightMotor);
		Serial.print("RealPower=");Serial.println(rightMotor);
    }
    _percentLeftMotor = ((realPower / 100) * (leftMotor / 100));
    _percentRightMotor = ((realPower / 100) * (rightMotor / 100));
}

float TextMotorCommandsInterpretter::getPercentLeft()
{
	return _percentLeftMotor;
}

float TextMotorCommandsInterpretter::getPercentRight()
{
	return _percentRightMotor;
}

boolean TextMotorCommandsInterpretter::getDirection()
{
	return _power > 0 ? true : false;
}

void TextMotorCommandsInterpretter::outputDebug(boolean debug)
{
	_debug = debug;
}

void TextMotorCommandsInterpretter::setCommandFormat(char startCharacter, char wordsBoundary, char endTerminator)
{
	char _startCharacter = startCharacter;
	char _wordsBoundary = wordsBoundary;
	char _endTerminator = endTerminator;
}

int TextMotorCommandsInterpretter::_getMotorDirection()
{
    byte splitPosition = _message.indexOf(':', 2);

    return _message.substring(2, splitPosition).toInt();
}

int TextMotorCommandsInterpretter::_getMotorPower()
{
    byte splitPosition = _message.indexOf(':', 2);
    byte endPosition = _message.indexOf(' ');
    Serial.print("endPosition=");Serial.println(endPosition);

    return _message.substring(splitPosition + 1, endPosition).toInt();
}
