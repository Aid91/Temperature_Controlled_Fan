// 
// 
// 

#include "InterruptBasedCommunication.h"
#include "BaseConstants.h"
#include "Helpers.h"

InterruptBasedCommunication::InterruptBasedCommunication()
{
	_running = false;
	_strRxFlag = 0;
	_i = 0;
	_count = 0;
	_old_i = 0;
	_lowTemperature = 0;
	_highTemperature = 0;
	_pwmRate = 0;
	
}

void InterruptBasedCommunication::init()
{
	cli();    //DISABLE ALL INTERRUPTIONS

			  //REGISTRE UBRR0
			  //9600 BAUD FOSC=16MHZ
	UBRR0 = 103;

	//REGISTRE USCR0C
	//COM ASYNCHRONE
	bitWrite(UCSR0C, UMSEL00, 0);
	bitWrite(UCSR0C, UMSEL01, 0);

	//PARITY NONE
	bitWrite(UCSR0C, UPM01, 0);
	bitWrite(UCSR0C, UPM00, 0);

	//8 DATA BIT
	bitWrite(UCSR0C, UCSZ02, 0);
	bitWrite(UCSR0C, UCSZ01, 1);
	bitWrite(UCSR0C, UCSZ00, 1);

	//REGISTRE UCSR0B  
	//RECEIVE & TRANSMITT ENABLE
	bitWrite(UCSR0B, RXEN0, 1);
	bitWrite(UCSR0B, TXEN0, 1);

	//ENABLE RX COMPLETE INTERRUPT
	bitWrite(UCSR0B, RXCIE0, 1);

	sei();    //ENABLE INTERRUPTION
}

boolean InterruptBasedCommunication::checkIfStartIsReceived()
{
	return _buffer[0] == 's' && _buffer[1] == 't' && _buffer[2] == 'a' && _buffer[3] == 'r' && _buffer[4] == 't';
}

boolean InterruptBasedCommunication::checkIfStopIsReceived()
{
	return _buffer[0] == 's' && _buffer[1] == 't' && _buffer[2] == 'o' && _buffer[3] == 'p';
}

void InterruptBasedCommunication::putstring(char* StringPtr)
// sends the characters from the string one at a time to the USART
{
	while (*StringPtr != 0x00)
	{
		send(*StringPtr);
		StringPtr++;
	}
}

void InterruptBasedCommunication::send(unsigned char data)
{
	//while the transmit buffer is not empty loop
	while (!(UCSR0A & (1 << UDRE0)));

	//when the buffer is empty write data to the transmitted  
	UDR0 = data;
}

int16_t InterruptBasedCommunication::parseInt(char* buff, uint8_t lenght)
{
	bool isNegative = false;
	int16_t value = 0;
	uint8_t m = 0;
	char c;

	do {
		c = buff[m];
		if (c == '-')
			isNegative = true;
		else if (c >= '0' && c <= '9')        // is c a digit?
			value = value * 10 + c - '0';
		m++;
	} while ((c >= '0' && c <= '9') && m < lenght);

	if (isNegative)
		value = -value;
	return value;
}

void InterruptBasedCommunication::handleInterrupt()
{
	_buffer[_i] = UDR0;     //Read USART data register
	if (_buffer[_i] == '#') //check for carriage return terminator and increment buffer index
	{
		// if terminator detected
		_strRxFlag = 1;    //Set String received flag 
						  //buffer[i - 1] = 0x00; //Set string terminator to 0x00
		_i = 0;        //Reset buffer index
		_old_i = 0;
		_count = 0;
	}
	else if (_buffer[_i] == ',')
	{
		if (_count >= 1)
		{
			memset(&_temp_buffer[0], 0, sizeof(_temp_buffer));

			for (int j = _old_i + 1; j < _i; ++j)
			{
				_temp_buffer[j - _old_i - 1] = _buffer[j];
			}

			if (_count == 1)
			{
				_lowTemperature = parseInt(_temp_buffer, _i - _old_i);
			}
			else if (_count == 2)
			{
				_highTemperature = parseInt(_temp_buffer, _i - _old_i);
			}
			else if (_count == 3)
			{
				_pwmRate = parseInt(_temp_buffer, _i - _old_i);
			}

		}

		_old_i = _i;
		_count++;
	}
	_i++;

	if (_strRxFlag == 1 && checkIfStartIsReceived())
	{
		_strRxFlag = 0;
		_running = true;
		_i = 0;        //Reset buffer index
		_old_i = 0;
		_count = 0;
	}
	else if (_strRxFlag == 1 && checkIfStopIsReceived())
	{
		_strRxFlag = 0;
		_running = false;
		_i = 0;        //Reset buffer index
		_old_i = 0;
		_count = 0;
	}
}

void InterruptBasedCommunication::SendTemperatureAndPwmValue(float temperature, uint8_t pwmValue)
{	
	putstring(BaseConstants::STX);
	putstring(Helpers::ConvertFloatToCharArray(temperature));
	putstring(BaseConstants::RS);
	putstring(Helpers::ConvertIntToCharArray(pwmValue));
	putstring(BaseConstants::ETX);
}

//INTERRUPT WHEN A BYTE COMING
ISR(USART0_RX_vect) {
	InterruptBasedCommunication::instance().handleInterrupt();
}