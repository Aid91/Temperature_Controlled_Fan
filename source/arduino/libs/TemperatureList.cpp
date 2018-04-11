// 
// 
// 

#include "TemperatureList.h"

void TemperatureList::Add(float temperature)
{
	 if (_count == 5)
	{
		_meanAvailable = true;
		_count = 0;
	}

	_temperatures[_count] = temperature;
	_count++;
}

float TemperatureList::CalculateMean()
{
	return (_temperatures[0] + _temperatures[1] + _temperatures[2] + _temperatures[3] + _temperatures[4]) / 5;
}
