#include "TemperatureControlledFan.h"

TemperatureControlledFan::TemperatureControlledFan(Communication communication, Adafruit_7segment matrix, uint8_t fanPin, uint8_t temperaturePin)
{
	_communication = communication;
	_fanPin = fanPin;
	_temperaturePin = temperaturePin;
	_matrix = matrix;
	_temperatureList = new TemperatureList();
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
	uint8_t calculatedPwmValue = (pwmPercentageRate * 255) / 100.0;
	analogWrite(fanPin, calculatedPwmValue);
}

void TemperatureControlledFan::ReadAndSetData()
{
	String command = "";  //Used to store the latest received command
	uint8_t serialResult = 0; //return value for reading operation method on serial in put buffer

	serialResult = _communication.ReadCommand(&command);
	if (serialResult == Constants::MSG_SUCCESS) {
		if (command == "start#") {
			// Read temperatures sent in the following order:
			// 1. Low temperature
			// 2. High temperature
			// 3. Automatic fan control
			// 4. Fan speed

			//serialResult = readSerialInputCommand(&command);

			//Serial.println("Start command received");
			_isStarted = !_isStarted;

			_lowTemperature = Serial.parseFloat();
			//Serial.println(lowTemperature);

			_highTemperature = Serial.parseFloat();
			//Serial.println(highTemperature);

			_pwmPercantageRate = Serial.parseInt();
			//Serial.println(pwmRation);

			if (_pwmPercantageRate == 0.0)
			{
				_automatic = 1;
			}
		}
		else if (command == "stop#")
		{
			_isStarted = !_isStarted;
		}
	}

	if (serialResult == Constants::NO_SERIAL_DATA_AVAIALABLE) {
		//Serial.println("No serial data available");
		//delay(500);
	}
	else {
		if (serialResult == Constants::SERIAL_IN_COMMAND_NOT_TERMINATED) {
			//Serial.println("Serial command not terminated");
			//delay(2000);
		}
	}
}

void TemperatureControlledFan::HandleStates()
{
	if (_isStarted) {
		// TODO: read temperature from the temperature sensorw

		float currentTemperature = ReadTemperature();

		float currentPwmRatio = CalculateCurrentFanSpeedBasedOnTemperature(currentTemperature);

		_communication.SendTemperatureAndPwmValue(currentTemperature, currentPwmRatio);

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

	if (_automatic != 1)
	{

		if (currentTemperature < _lowTemperature)
		{
			currentPwmRatio = 0.0;
		}
		else if (currentTemperature > _lowTemperature && currentTemperature < _highTemperature)
		{
			currentPwmRatio = _pwmPercantageRate;
		}
		else if (currentTemperature > _highTemperature)
		{
			if (_pwmPercantageRate <= 50)
			{
				currentPwmRatio = _pwmPercantageRate * 2;
			}
			else
			{
				currentPwmRatio = 100;
			}
			
		}
	}
	else
	{
		if (currentTemperature > _lowTemperature)
		{
			currentPwmRatio = map(currentTemperature, _lowTemperature, _highTemperature, 51, 255) * 100 / 255.0;
		}
		else if(currentTemperature > _highTemperature)
		{
			currentPwmRatio = 100;
		}
	}

	return currentPwmRatio;
}

void TemperatureControlledFan::Init()
{
	Serial.begin(9600);
	pinMode(_fanPin, OUTPUT);
	_matrix.begin(0x70);
}
