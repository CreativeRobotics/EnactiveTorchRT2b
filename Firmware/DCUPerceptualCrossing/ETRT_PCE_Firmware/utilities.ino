
void enablePeripheralPower(){
  peripheralPowerOn = true;
  digitalWrite(PIN_PERIPHERAL_POWER_ENABLE, peripheralPowerOn);
}
void disablePeripheralPower(){
  peripheralPowerOn = false;
  digitalWrite(PIN_PERIPHERAL_POWER_ENABLE, peripheralPowerOn);
}

void enable5VPower(){
  v5PowerOn = true;
  digitalWrite(PIN_5V_ENABLE, v5PowerOn);
}
void disable5VPower(){
  v5PowerOn = false;
  digitalWrite(PIN_5V_ENABLE, v5PowerOn);
}


void initLEDs(){
  leds.begin();
  leds.setBrightness(32);
  setLEDs(100, 200, 255);
  //setLED(1, blueLED);
  //leds.show();
}

void setLED(uint16_t ledNo, const uint8_t ledcol[]){
  pauseTrackerball();
  //stopTrackerball();
  leds.setPixelColor(ledNo, ledcol[LRED], ledcol[LGREEN], ledcol[LBLUE]);
  leds.show();
 // startTrackerball(true, -1, -1); //swap XY, xdirection, ydirection
  resumeTrackerball();
}

void setLEDs(uint8_t r, uint8_t g, uint8_t b){
  pauseTrackerball();
  //stopTrackerball();
  for(int n = 0; n < LED_QUANTITY; n++){
    leds.setPixelColor(n, r, g, b);
    leds.show();
  }
  //startTrackerball(true, -1, -1); //swap XY, xdirection, ydirection
  resumeTrackerball();
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return leds.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return leds.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return leds.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
