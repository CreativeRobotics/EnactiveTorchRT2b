
#include "Table.h"
#include <string.h>


float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


float Table::getValue(uint16_t index){
	if(index >= TABLE_LENGTH) return values[TABLE_LENGTH-1];
	return values[index];
}

float Table::mapValue(float input){
	int mapped = (int)mapfloat(input, inMin, inMax, 0.0, 100.0);
	if(mapped < 0) mapped = 0;
	if(mapped >= TABLE_LENGTH) mapped =TABLE_LENGTH-1;
	return values[mapped];
}
