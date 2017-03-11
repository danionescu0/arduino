#ifndef EncryptedSoftwareSerial_h
#define EncryptedSoftwareSerial_h

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include "SoftwareSerial.h"

class EncryptedSoftwareSerial {
    public:
	EncryptedSoftwareSerial(uint8_t rx, uint8_t tx, int baud, uint8_t* key, String DEVICE_CODE);
        boolean parseIncomming();
        String getDecrypted();
        void transmit(String message);
        
    private:
        uint8_t _rx;
        uint8_t _tx;
        char _buffer[19];
        uint8_t* _key;
        byte _bufferSize = 19;
        char _data[16];
        const char _TERMINATOR = '|';
        String _DEVICE_CODE;

        SoftwareSerial _externalSerial;

        void clearBuffer();
        boolean isForThisDevice();

};

#endif
