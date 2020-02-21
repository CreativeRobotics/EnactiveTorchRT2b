//driver for switch and LED

boolean checkSwitch(){
  //Switch input is active LOW
  return !digitalRead(USER_SWITCH_PIN);
}

void setUserLED(uint8_t ledVal){
  //assume a 10 bit PWM range ... because of the haptics library
  //analogWrite(USER_LED_PIN, ledVal);
  setLEDs(0, ledVal, 0);
}

void userLED_ON(){
  //analogWrite(USER_LED_PIN, 1023);
  //digitalWrite(USER_LED_PIN, 1);
  if(ledState) return;
  ledState = 1;
  setLEDs(255, 0, 0);
  //LEDBrightness = 1023;
}

void userLED_OFF(){
  if(!ledState) return;
  ledState = 0;
  setLEDs(0, 0, 0);
  //analogWrite(USER_LED_PIN, 0);
  
  //digitalWrite(USER_LED_PIN, 0);
  LEDBrightness = 0;
}

void flash(unsigned long duration){
	userLED_ON();
	flashActive = 1;
	flashStart = millis();
  flashEnd = flashStart+ duration;
}

void updateFlash(){
  
  if(!flashActive) return;
  if(millis() > flashEnd){
		userLED_OFF();
		flashActive = 0;
	}
}
