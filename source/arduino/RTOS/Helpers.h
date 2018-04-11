// Helpers.h

#ifndef _HELPERS_h
#define _HELPERS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Helpers
{
public:
	static char* ConvertFloatToCharArray(float value);
	static char* ConvertIntToCharArray(int value);
};

#endif

