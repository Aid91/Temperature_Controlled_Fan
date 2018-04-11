#include "TemperatureList.h"
#include "TemperatureControlledFan.h"
#include "Constants.h"
#include "Communication.h"

#define TEMPERATURE_PIN 0                      
#define SERIAL_BAUDRATE 9600            
#define FAN_PIN 3

Communication communication = Communication();
Adafruit_7segment matrix = Adafruit_7segment();
TemperatureControlledFan temperatureControlledFan = TemperatureControlledFan(communication, matrix, FAN_PIN, TEMPERATURE_PIN);

void setup() {
	// put your setup code here, to run once:
	// initialize the serial communication:
	temperatureControlledFan.Init();
	
	//TCCR2B = (TCCR2B & 0b11111000) | 0x01;
}


/*****************************************************************
The loop method is executed forever right after the setup method
is finished.
******************************************************************/
void loop() {

	temperatureControlledFan.ReadAndSetData();

	temperatureControlledFan.WriteToLED();

	temperatureControlledFan.HandleStates();

	delay(1000);
}
