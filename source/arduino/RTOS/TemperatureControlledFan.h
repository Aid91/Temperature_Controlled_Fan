// TemperatureControlledFan.h

#ifndef _TEMPERATURECONTROLLEDFAN_h
#define _TEMPERATURECONTROLLEDFAN_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include "TemperatureList.h"

class TemperatureControlledFan
{
public:
	TemperatureControlledFan() = default;
	TemperatureControlledFan(uint8_t fanPin, uint8_t temperaturePin, Adafruit_7segment matrix);
	void Init();
	float ReadTemperature();
	void CalculatePwmAndControlFan(uint8_t pwmPercentageRate, uint8_t fanPin);
	void HandleStates();
	void TemperatureControlledFan::WriteToLED();
	float CalculateCurrentFanSpeedBasedOnTemperature(float tempeature);
	~TemperatureControlledFan()
	{
		if (_temperatureList != nullptr)
		{
			delete _temperatureList;
		}
	}
private:
	uint8_t _temperaturePin;
	uint8_t _fanPin;
	Adafruit_7segment _matrix;
	TemperatureList* _temperatureList;
};

#endif


