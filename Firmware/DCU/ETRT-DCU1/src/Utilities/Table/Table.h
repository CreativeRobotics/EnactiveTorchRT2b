#ifndef Table_h
#define Table_h

#include <Arduino.h>
#include <string.h>

#define TABLE_LENGTH 101
//lookup table class
/*
An input value (any float) can be mapped to an output value using a lookup table
The iput value is scaled and capped using the map function.
EG if the min value is 10 and the max is 200 then 10 is index 0 and 200 is index 99
If the value sent is 210 then it is still index 99, if it is 2 then the index is still 0
*/
class Table{
public:
	Table();
	float getValue(uint16_t index);
	float mapValue(float input);
private:
	float values[TABLE_LENGTH];
	float inMin = 0.0, inMax = 100.0;
};
#endif