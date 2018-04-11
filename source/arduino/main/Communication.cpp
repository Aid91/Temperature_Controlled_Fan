// 
// 
// 

#include "Communication.h"
#include "Constants.h"

uint8_t Communication::ReadCommand(String * command)
{
	uint8_t status = Constants::MSG_SUCCESS;

	//check if serial data is available for reading
	if (Serial.available()) {
		char serialInByte;//temporary variable to hold the last serial input buffer character

		do {//Read serial input buffer data byte by byte 
			serialInByte = Serial.read();
			*command = *command + serialInByte;//Add last read serial input buffer byte to *command pointer
		} while (serialInByte != '#' && Serial.available());//until '#' comes up or no serial data is available anymore

		if (serialInByte != '#') {
			status = Constants::SERIAL_IN_COMMAND_NOT_TERMINATED;
		}
	}
	else {//If not serial input buffer data is AVAILABLE, operationStatus becomes WRG_NO_SERIAL_DATA_AVAILABLE (= No data in the serial input buffer AVAILABLE)
		status = Constants::NO_SERIAL_DATA_AVAIALABLE;
	}

	return status;
}

void Communication::SendTemperatureAndPwmValue(float temperature, uint8_t pwmValue)
{
	Serial.print(Constants::STX);
	Serial.print(temperature);
	Serial.print(Constants::RS);
	Serial.print(pwmValue);
	Serial.print(Constants::ETX);
}
