void initPins(){

  pinMode(TRACKERBALL_XA, INPUT_PULLUP);
  pinMode(TRACKERBALL_XB, INPUT_PULLUP);
  
  pinMode(TRACKERBALL_YA, INPUT_PULLUP);
  pinMode(TRACKERBALL_YB, INPUT_PULLUP);


  //pinMode(LCD4, OUTPUT);
  //pinMode(LCD5, OUTPUT);
  //pinMode(LCD6, OUTPUT);
  //pinMode(LCD7, OUTPUT);
  //pinMode(LCDRS, OUTPUT);
  //pinMode(LCDEN, OUTPUT);
  //pinMode(LCDRW, OUTPUT);
  //pinMode(VIB1ENABLE, OUTPUT);
  //pinMode(VIB1PWMPIN, OUTPUT);
  //pinMode(VIB2ENABLE, OUTPUT);
  //pinMode(VIB2PWMPIN, OUTPUT);

  pinMode(USER_SWITCH_PIN, INPUT_PULLUP);
  //pinMode(USER_LED_PIN, OUTPUT);
  pinMode(SOUNDER_PIN, OUTPUT);
  pinMode(PIN_PERIPHERAL_POWER_ENABLE, OUTPUT);
  pinMode(PIN_5V_ENABLE, OUTPUT);
  disablePeripheralPower();
  disable5VPower();
}

/*
void initLCD(){
  // 7 pin connection (fast): normal LCD, single HD44780 controller
  //LiquidCrystalFast(uint8_t rs, uint8_t rw, uint8_t enable, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7) 
  #ifdef LCDOK
    lcd.begin(nColumns, nRows);
    // Print a message to the LCD.
    
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("System Online!");
    //lcd.setCursor(0, 1);
    //if(!USB) lcd.print("NO USB!");
    //else lcd.print("USB Connected!");
  #endif
}*/
