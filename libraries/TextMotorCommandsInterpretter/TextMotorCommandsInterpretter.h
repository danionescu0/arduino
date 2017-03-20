#ifndef TextMotorCommandsInterpretter_h
#define TextMotorCommandsInterpretter_h

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class TextMotorCommandsInterpretter {
    public:
		TextMotorCommandsInterpretter(int minX, int maxX, int minY, int maxY);
		void analizeText(String buffer);
		void outputDebug(boolean debug);
		float getPercentLeft();
		float getPercentRight();
		boolean getDirection();
        
    private:
		int _getMotorDirection();
		int _getMotorPower();
		String _message;
		float _percentLeftMotor;
		float _percentRightMotor;
		boolean _debug;
		int _power;
		int _minX;
		int _maxX;
		int _minY;
		int _maxY;
};

#endif
