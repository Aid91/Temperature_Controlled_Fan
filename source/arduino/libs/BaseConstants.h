// Constants.h

#ifndef _BASECONSTANTS_h
#define _BASECONSTANTS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class BaseConstants {
public:
	static char* STX; 
	static char* ETX;
	static char* RS;
};

#endif

