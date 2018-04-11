// Communication.h

#ifndef _COMMUNICATION_h
#define _COMMUNICATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif
#include "WString.h"

class Communication
{	
public:
	uint8_t ReadCommand(String* command);
	void SendTemperatureAndPwmValue(float temperature, uint8_t pwmValue);
};

#endif

