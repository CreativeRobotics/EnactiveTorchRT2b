void printDataStream(){
  if(millis() < nextPrintTime) return;
  nextPrintTime = millis() + printInterval;
  Serial.print("X=");
  Serial.print(xCountTotal);
  Serial.print(" Y=");
  Serial.print(yCountTotal);
  buttonState ? Serial.print(" Button ON") : Serial.print(" Button OFF");
  Serial.println();
}
void printData(){
  //print a standard data packet
  //Collect the variables for encoders now so they don't change half way through the print functions
  //Send encoder values as a relative change since the last time
  //long printStart = micros();
  //TIME TEST - from here until USB.send_now() returns takes 95 microseconds
  if(!USB) return;
  long tempX;
  long tempY;
  long xChange, yChange;
  unsigned long elapsedTime;
  boolean tempButtonState = 0;
  int tempLEDVal = 0;
  //capture the encoder states
  tempX = xCountTotal;
  tempY = yCountTotal;

  //work out the change
  xChange = tempX-oldXCount;
  yChange = tempY-oldYCount;
  //copy button and LED states
  tempButtonState = buttonState;
  tempLEDVal = LEDBrightness;
  //Get elapsed Time
  elapsedTime = millis() - dataSendTimer;
  dataSendTimer = millis();
  //update the old values to the new ones.
  oldXCount = tempX;
  oldYCount = tempY;


  String xData = String(xChange);
  String yData = String(yChange);
  String buttonStateData = String(buttonState);
  String ledValData = String(tempLEDVal);
  String elapsedT = String(elapsedTime);
  String data = elapsedT + ',' + xData + ',' + yData + ',' + buttonStateData + ',' + ledValData;
  
  USB.println(data);
  USB.flush(); //this stops interrupts happening
  //USB.println(micros()-printStart);
  /*USB.print(elapsedTime);
  USB.write(COMMA); USB.print(xChange);
  USB.write(COMMA); USB.print(yChange);
  USB.write(COMMA); 
  if(tempButtonState) USB.print("ON");
  else USB.print("OFF");
  USB.write(COMMA); 
  USB.print(tempLEDVal);
  USB.println();*/
}

void printData2(){
  if(!USB) return;
                    USB.print(hapticRate1);
  USB.write(COMMA); USB.print(hapticRate2);
  USB.write(COMMA); USB.print(LCDText);
  USB.println();
}
