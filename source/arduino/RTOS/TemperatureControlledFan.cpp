#include "TemperatureControlledFan.h"
#include "InterruptBasedCommunication.h"

TemperatureControlledFan::TemperatureControlledFan(uint8_t fanPin, uint8_t temperaturePin, Adafruit_7segment matrix)
{
	_fanPin = fanPin;
	_temperaturePin = temperaturePin;
	_matrix = matrix;
	_temperatureList = new TemperatureList();
}

void TemperatureControlledFan::Init()
{
	pinMode(_fanPin, OUTPUT);
	_matrix.begin(0x70);
}


float TemperatureControlledFan::ReadTemperature()
{
	uint16_t value = analogRead(_temperaturePin);
	float kelvin = (value / 1024.0) * 5000;
	float celsius = kelvin / 10 - 273.15;
	
	float meanTemperature = 0.0;

	_temperatureList->Add(celsius);

	if (_temperatureList->isMeanAvailable())
	{
		meanTemperature = _temperatureList->CalculateMean();
	}
	else
	{
		meanTemperature = 0;
	}
	return meanTemperature;
}

void TemperatureControlledFan::WriteToLED()
{
	float temperature = 0.0;

	if (_temperatureList->isMeanAvailable())
	{
		temperature = _temperatureList->CalculateMean();
	}
	else
	{
		temperature = 0;
	}
	_matrix.print(temperature);
	_matrix.writeDisplay();
}


void TemperatureControlledFan::CalculatePwmAndControlFan(uint8_t pwmPercentageRate, uint8_t fanPin)
{
	uint8_t calculatedPwmValue = (pwmPercentageRate * 255) / 100;
	analogWrite(fanPin, calculatedPwmValue);
}

void TemperatureControlledFan::HandleStates()
{
	if (InterruptBasedCommunication::instance().isRunning()) {
		// TODO: read temperature from the temperature sensorw

		float currentTemperature = ReadTemperature();

		float currentPwmRatio = CalculateCurrentFanSpeedBasedOnTemperature(currentTemperature);

		InterruptBasedCommunication::instance().SendTemperatureAndPwmValue(currentTemperature, currentPwmRatio);

		CalculatePwmAndControlFan(currentPwmRatio, _fanPin);
	}
	else
	{
		CalculatePwmAndControlFan(0, _fanPin);
	}
}

float TemperatureControlledFan::CalculateCurrentFanSpeedBasedOnTemperature(float currentTemperature)
{
	float currentPwmRatio = 0.0F;

	if (InterruptBasedCommunication::instance().isAutomatic() != 1)
	{
		if (currentTemperature < InterruptBasedCommunication::instance().getLowTemperature())
		{
			currentPwmRatio = 0.0;
		}
		else if (currentTemperature > InterruptBasedCommunication::instance().getLowTemperature() && currentTemperature < InterruptBasedCommunication::instance().getHighTemperature())
		{
			currentPwmRatio = InterruptBasedCommunication::instance().getPwmRate();
		}
		else if (currentTemperature > InterruptBasedCommunication::instance().getHighTemperature())
		{
			if (InterruptBasedCommunication::instance().getPwmRate() <= 50)
			{
				currentPwmRatio = InterruptBasedCommunication::instance().getPwmRate() * 2;
			}
			else
			{
				currentPwmRatio = 100;
			}
		}
	}
	else
	{
		if (currentTemperature > InterruptBasedCommunication::instance().getLowTemperature())
		{
			currentPwmRatio = map(currentTemperature, InterruptBasedCommunication::instance().getLowTemperature(), InterruptBasedCommunication::instance().getHighTemperature(), 51, 255) * 100 / 255.0;
		}
		else if (currentTemperature > InterruptBasedCommunication::instance().getHighTemperature())
		{
			currentPwmRatio = 100;
		}
	}

	return currentPwmRatio;
}
