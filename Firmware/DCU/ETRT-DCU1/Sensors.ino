//Dual ranger sensor


void initTOFSensor(){
  Sensor.begin(A4);
}
void updateTOFSensor(){
  Sensor.update();
}

//#define TRIG_PIN PA03//ADC0
//#define PWM_PIN  PB05//ADC2
void initSonarSensor(){
  if(DEBUGGING) DEBUG.println("init Sonar ...");
  if(device.sonarMode == SONAR_SERIAL_MODE){
    //setup the sonar object
    Sonar.begin();
  }
  //setupSonar(); //PULSE IN on  A2
}

void updateSonar(){
  if(device.sonarMode == SONAR_SERIAL_MODE){
    //update the sonar object
    Sonar.update();
  }  else device.inputs[2] = getSonarRange();
}


/*
void setupSonar(){
  pinMode(pulsePin, INPUT);
  //pinMode(enPin, OUTPUT);
  //attachInterrupt(digitalPinToInterrupt(pulsePin), pulsePin_isr, CHANGE);
  
  //digitalWrite(enPin, HIGH);
}*/


float getSonarRange(){
  //147 microseconds per inch
  range = (float)pulseLength;
  range /=147.0;
  range *=2.54;
  return range;
}
void sonarPulsePin_isr(){
  if(digitalRead(pulsePin)) riseTime = micros();
  else pulseLength = micros()-riseTime;
  sonarInts++;
}
