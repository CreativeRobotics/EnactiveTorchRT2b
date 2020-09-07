//Driver for general purpose Sensor Head 
//ETRT2
#ifndef SONARSENSOR_H
#define SONARSENSOR_H

#include "Arduino.h"

#define SSerial SENSOR_SERIAL //serial2
#define SENSOR_BAUD 9600 

class SonarSensor {
public:
	void begin();
	void reset();
	void update();
	float getRange()		{return range;}
	float getDelta()		{return rangeDelta;}
	uint32_t getUpdates() {return sonarUpdates;}
	bool debugStream = false;
private:
	bool updateSerial();
	void updateTimeDelta();
	bool isNumber(uint8_t dta);
	unsigned long lastUpdate;
	float deltaT = 0.01;
	float range; //combined value
	float lastRange = 0;
	float rangeDelta;
	String inData = "";
	uint8_t parseState = 0;
	uint32_t sonarUpdates = 0;
};

#endif // SONARSENSOR_H