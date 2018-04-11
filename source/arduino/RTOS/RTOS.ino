#include "TemperatureList.h"
#include "Helpers.h"
#include "InterruptBasedCommunication.h"
#include "TemperatureControlledFan.h"
#include <Arduino_FreeRTOS.h>

#include <semphr.h>  // add the FreeRTOS functions for Semaphores (or Flags).

#define TEMPERATURE_PIN 0                              
#define FAN_PIN 3

// Declare a mutex Semaphore Handle which we will use to manage the Temperature Controlled Fan object.
// It will be used to ensure only only one Task is accessing this resource at any time.
SemaphoreHandle_t xTemperatureControlledFanSemaphore;

// define two Tasks for TaskLedIndication & taskAnalogReadAndControl
void TaskLedIndication(void *pvParameters);
void TaskAnalogReadAndControl(void *pvParameters);

Adafruit_7segment matrix = Adafruit_7segment();
TemperatureControlledFan temperatureControlledFan = TemperatureControlledFan(FAN_PIN, TEMPERATURE_PIN, matrix);

void setup() {
	
	temperatureControlledFan.Init();
	InterruptBasedCommunication::instance().init();
	// Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
	// because it is sharing variables such as _temperatureList object in our case
	// Semaphores should only be used whilst the scheduler is running, but we can set it up here.
	if (xTemperatureControlledFanSemaphore == NULL)  
	{
		xTemperatureControlledFanSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Temperature Controlled Fan object
		if ((xTemperatureControlledFanSemaphore) != NULL)
			xSemaphoreGive((xTemperatureControlledFanSemaphore));  // Make the Temperature Controlled Fan object available for use, by "Giving" the Semaphore.
	}

	// Now set up two Tasks to run independently.
	xTaskCreate(
		TaskLedIndication
		, (const portCHAR *)"TaskLedIndication"  // A name just for humans
		, 128  // This stack size can be checked & adjusted by reading the Stack Highwater
		, NULL
		, 1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
		, NULL);

	xTaskCreate(
		TaskAnalogReadAndControl
		, (const portCHAR *) "AnalogReadAndControl"
		, 128  // Stack size
		, NULL
		, 2  // Priority
		, NULL);

	// Now the Task scheduler, which takes over control of scheduling individual Tasks, is automatically started.
}

void loop()
{
	// Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskLedIndication(void *pvParameters __attribute__((unused)))  // This is a Task.
{
	for (;;) // A Task shall never return or exit.
	{
		if (xSemaphoreTake(xTemperatureControlledFanSemaphore, (TickType_t)2) == pdTRUE)
		{
			temperatureControlledFan.WriteToLED();
			xSemaphoreGive(xTemperatureControlledFanSemaphore); // Now free or "Give" the Temperature Controlled Fan object for others.
		}
		vTaskDelay(1000 / portTICK_PERIOD_MS); 
	}
}

void TaskAnalogReadAndControl(void *pvParameters __attribute__((unused)))  // This is a Task.
{
	for (;;)
	{
		if (xSemaphoreTake(xTemperatureControlledFanSemaphore, (TickType_t)2) == pdTRUE)
		{
			temperatureControlledFan.HandleStates();
			xSemaphoreGive(xTemperatureControlledFanSemaphore); // Now free or "Give" the Temperature Controlled Fan object for others.
		}
		
		vTaskDelay(1000 / portTICK_PERIOD_MS); 
	}
}
