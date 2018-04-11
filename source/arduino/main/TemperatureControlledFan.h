// TemperatureControlledFan.h

#ifndef _TEMPERATURECONTROLLEDFAN_h
#define _TEMPERATURECONTROLLEDFAN_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "Communication.h"
#include "Constants.h"
#include "TemperatureList.h"

#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"


class TemperatureControlledFan
{
public:
	TemperatureControlledFan() = default;
	TemperatureControlledFan(Communication communication, Adafruit_7segment matrix, uint8_t fanPin, uint8_t temperaturePin);
	float ReadTemperature();
	void WriteToLED();
	void CalculatePwmAndControlFan(uint8_t pwmPercentageRate, uint8_t fanPin);
	void ReadAndSetData();
	void HandleStates();
	float CalculateCurrentFanSpeedBasedOnTemperature(float tempeature);
	void Init();

	inline bool GetIsStarted() { return _isStarted; }
	inline void SetIsStarted(bool isStarted) { _isStarted = isStarted; }

	inline int8_t GetLowTemperature() { return _lowTemperature; }
	inline void SetLowTemperature(int8_t lowTemperature) { _lowTemperature = lowTemperature; }

	inline int8_t GetHighTemperature() { return _highTemperature; }
	inline void SetHighTemperature(int8_t highTemperature) { _highTemperature = highTemperature; }

	inline uint8_t GetPwmPercantageRate() { return _pwmPercantageRate; }
	inline void SetPwmPercantageRate(uint8_t pwmPercantageRate) { _pwmPercantageRate = pwmPercantageRate; }

	inline bool GetAutomatic() { return _automatic; }
	inline void SetAutomatic(bool automatic) { _automatic = automatic; }

	~TemperatureControlledFan()
	{
		if (_temperatureList != nullptr)
		{
			delete _temperatureList;
		}
	}

private:
	bool _isStarted;
	int8_t _lowTemperature;
	int8_t _highTemperature;
	uint8_t _pwmPercantageRate;
	bool _automatic;
	uint8_t _temperaturePin;
	uint8_t _fanPin;
	Communication _communication;
	Adafruit_7segment _matrix;
	TemperatureList* _temperatureList;
};

#endif


