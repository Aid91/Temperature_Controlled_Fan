// InterruptBasedCommunication.h

#ifndef _INTERRUPTBASEDCOMMUNICATION_h
#define _INTERRUPTBASEDCOMMUNICATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class InterruptBasedCommunication
{
public:
	void init();
	boolean checkIfStartIsReceived();
	boolean checkIfStopIsReceived();
	void putstring(char* StringPtr);
	void send(unsigned char data);
	int16_t parseInt(char* buff, uint8_t lenght);

	inline bool isRunning() { return _running; }
	void handleInterrupt();

	inline bool isAutomatic() { return _pwmRate == 0; }
	inline int8_t getLowTemperature() { return _lowTemperature; }
	inline int8_t getHighTemperature() { return _highTemperature; }
	inline uint8_t getPwmRate() { return _pwmRate; }

	void SendTemperatureAndPwmValue(float temperature, uint8_t pwmValue);

	static InterruptBasedCommunication& instance()
	{
		static InterruptBasedCommunication INSTANCE;
		return INSTANCE;
	}

private:
	InterruptBasedCommunication();
	volatile boolean _running;
	volatile int8_t _buffer[20];
	char _temp_buffer[20];
	volatile uint8_t _strRxFlag;
	volatile uint8_t _i;
	volatile uint8_t _count;
	volatile uint8_t _old_i;
	int8_t _lowTemperature;
	int8_t _highTemperature;
	uint8_t _pwmRate;
};


#endif

