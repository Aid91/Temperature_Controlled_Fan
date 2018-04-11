// Constants.h

#ifndef _CONSTANTS_h
#define _CONSTANTS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "WString.h"
#include "BaseConstants.h"

class Constants : public BaseConstants {
public:
	static const int MSG_SUCCESS=0;
	static const int NO_SERIAL_DATA_AVAIALABLE = 1;
	static const int SERIAL_IN_COMMAND_NOT_TERMINATED = 2;
};

#endif

