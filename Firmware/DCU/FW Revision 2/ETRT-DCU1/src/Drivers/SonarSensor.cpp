#include "SonarSensor.h"


void SonarSensor::begin(){
	SENSOR_SERIAL.begin(SENSOR_BAUD);
	reset();
}
//-----------------------------------------------------

void SonarSensor::reset(){
	lastUpdate = micros();
	deltaT = 0.01;
	range = 0.0; //combined value
	lastRange = 0.0;
	rangeDelta = 0.0;
	parseState = 0;
	inData = "";
}
//-----------------------------------------------------

void SonarSensor::update(){

	if(updateSerial()){
		updateTimeDelta();
		lastRange = range;
		range = inData.toFloat();
		rangeDelta = (range-lastRange)/deltaT;
		lastUpdate = micros();
	}
}
//-----------------------------------------------------

bool SonarSensor::updateSerial(){
	//Parse serial port data
	uint8_t inByte;
  while(SSerial.available()){
    inByte = SSerial.read();
		if(debugStream) Serial.write(inByte);
		sonarUpdates++;
		switch(parseState){
		case 0:
			if(inByte == 'R') parseState++;
			break;
		case 1:
			if(isNumber(inByte)){
				inData = "";
				inData.concat((char)inByte);
				parseState++;
			}else	parseState = 0;
		case 2:
			if(isNumber(inByte)){
				inData.concat((char)inByte);
				parseState++;
			}else	parseState = 0;
		case 3:
			if(isNumber(inByte)){
				inData.concat((char)inByte);
				parseState++;
			}else	parseState = 0;
		case 4:
		//Check that this byte is valid end of packet
			if(inByte != 13){
				parseState = 0;
				return false;
			}
			sonarUpdates++;
			//collected three bytes, return true
			parseState = 0;
			return true;
		}
	}
	return false;
}
//-----------------------------------------------------

bool SonarSensor::isNumber(uint8_t dta){
	if(dta < 48 || dta > 57) return false;
	return true;
}

void SonarSensor::updateTimeDelta(){
	deltaT =  ((float)micros()-(float)lastUpdate)/1000000.0;
	if(deltaT == 0.0) deltaT = 0.0000001; //.1 microsecond
}
//-----------------------------------------------------
