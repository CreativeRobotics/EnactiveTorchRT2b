//Piezo sounder drivers
//int beepTimer = 0;
//int beepLimit = 0;

void beepX(unsigned long bduration){
  //blocking version for startup
  digitalWrite(SOUNDER_PIN, 1);
  for(unsigned long n = 0; n < bduration; n++) delay(1);
  digitalWrite(SOUNDER_PIN, 0);
}
void beep(unsigned long bduration){
 // tone(SOUNDER_PIN, ppitch, duration);
  beepON();
  beepActive = true;
  beepStart = millis();
  beepEnd = beepStart+ bduration;
  //Serial.print("Beeping for: ");
  //Serial.println(bduration);
}

void updateBeep(){
  if(!beepActive) return;
  if(millis() > beepEnd){
    beepOFF();
    beepActive = 0;
  }
  
}


void beepON(){
  if(beepState == 1) return;
  //Serial.println("Beep ON");
  beepState = 1;
  digitalWrite(SOUNDER_PIN, beepState);
}

void beepOFF(){
  if(beepState == 0) return;
  beepState = 0;
  beepActive = 0;
  digitalWrite(SOUNDER_PIN, beepState);
  //Serial.println("Beep OFF");
}
