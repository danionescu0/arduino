#include <EncryptedSoftwareSerial.h>
#include <SoftwareSerial.h>
#include <AESLib.h>

EncryptedSoftwareSerial::EncryptedSoftwareSerial(uint8_t rx, uint8_t tx, int baud, uint8_t* key, String DEVICE_CODE)
	: _externalSerial(rx, tx)
{
	_rx = rx;
	_tx = tx;
	_externalSerial.begin(baud);
	_externalSerial.setTimeout(100);
	_key = key;
	_DEVICE_CODE = DEVICE_CODE;
}

void EncryptedSoftwareSerial::transmit(String message)
{
	_externalSerial.print(message);
}

String EncryptedSoftwareSerial::getDecrypted()
{
	int i;
    for (i=0;i<=15;i++) {
        _data[i] = _buffer[i+3];
    }
    aes128_cbc_dec(_key, _key, _data, 16);
    String result = "";
    for (i=0;i<=15;i++) {
        if (_data[i] == _TERMINATOR) {
            return result;
        }
        result += _data[i];
    }

    return result;
}

boolean EncryptedSoftwareSerial::parseIncomming()
{
    if (_externalSerial.available() > 0) {
    	_externalSerial.readBytes(_buffer, 19);
    	return true;
        if (isForThisDevice()) {
            return true;
        }
        clearBuffer();
    }

    return false;
}

boolean EncryptedSoftwareSerial::isForThisDevice()
{
    String incommingDeviceCode = "";
    incommingDeviceCode += _buffer[0];
    incommingDeviceCode += _buffer[1];

    return incommingDeviceCode == _DEVICE_CODE;
}

void EncryptedSoftwareSerial::clearBuffer()
{
    for (int i=0; i<=_bufferSize - 1; i++) {
        _buffer[i] = ' ';
    }
}
