// 
// 
// 

#include "Helpers.h"

char * Helpers::ConvertFloatToCharArray(float value)
{
	char buffer[10];
	dtostrf(value, 4, 3, buffer);
	return buffer;
}

char * Helpers::ConvertIntToCharArray(int value)
{
	char buffer[10];
	int ret = sprintf(buffer, "%d", value);
	return buffer;
}
