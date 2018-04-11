// TemperatureList.h

#ifndef _TEMPERATURELIST_h
#define _TEMPERATURELIST_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


class TemperatureList
{
public:
	TemperatureList() : _count(0), _meanAvailable(false) {}
	void Add(float temperature);
	float CalculateMean();
	inline boolean isMeanAvailable() { return _meanAvailable; }
	inline boolean count() { return _count; }
private:
	float _temperatures[5];
	int _count = 0;
	boolean _meanAvailable;
};


#endif

