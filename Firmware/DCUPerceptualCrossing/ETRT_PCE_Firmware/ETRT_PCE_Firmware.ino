//

//PCE Firmware for enactive torch
//#include <LiquidCrystalFast.h>
#include "src\Utilities\Bounce2\Bounce2.h"
#include <SamRTC.h> //In CR core lib
//#include <SPI.h>  //In CR core lib
#include "src\Drivers\Adafruit_NeoPixel.h"
#include "wiring_private.h"

#include "src\Commander\Commander.h"
#include "src\Commander\prefabs\SDCards\PrefabFileNavigator.h"

//Real Time Clock
//#include <RV8523.h>
//RV8523 rtc;


//#include <SdFat.h>
//#include <SdFatConfig.h>
//#include <sdios.h>
//#include <SysCall.h>
//#include "src\Drivers\IMU\MPU9250_RegisterMap.h"
//#include "src\Drivers\IMU\SparkFunMPU9250-DMP.h"


#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Wire.h>

#include "haptics.h"
#include "Variables.h"
#include "Pins.h"

Commander cmdDCU;
extern const commandList_t  DCUCommands[]; //forward declare the master command list
extern const uint16_t       numOfDCUCmds; //This is a forward declarationso the compiler knows we are going to declare this variable properly later


#define LED_QUANTITY 17

Adafruit_NeoPixel leds(LED_QUANTITY, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);
//adafruit display
#define TFT_DC 26u //PB01
#define TFT_CS PB04
#define TFT_MOSI 27u
#define TFT_CLK 28u
// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(SPI_CS, TFT_DC);
// If using the breakout, change pins as desired
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
//Adafruit_ILI9341 tft = Adafruit_ILI9341(-1, 26u, 27u, 28u, -1, -1);


unsigned long iterations = 0;


bool trialActive = 0;
unsigned long updateDisplay = 100;

uint8_t ledR = 0, ledG = 200, ledB = 100;

void setup() {
  initPins();
  delay(50);
  enable5VPower();
  Serial.begin(115200);
  cmdDCU.begin(&Serial, DCUCommands, numOfDCUCmds);
  cmdDCU.commanderName = "ETRT-PCE>";
  cmdDCU.errorMessages(OFF);
  delay(500);
  tft.begin(120000000);//24000000
  //setupAnimate();
  //analogWriteResolution(10);
  //analogWriteFrequency(VIB1PWMPIN, HPWMFreq);
  
  //initLCD(); //hangs here
  //haptic1.initialise(H1PWMPIN, H1ENABLE);
  //haptic2.initialise(H2PWMPIN, H2ENABLE);

  //beepON();
  //userLED_ON();
  while(!Serial){;}
  beepON();
  delay(500);
  USB.println("ETRT PCE Started . . .");
  beepOFF();
  delay(500);
  startTrackerball(true, -1, -1); //swap XY, xdirection, ydirection
  resetTrackerball();
  initLEDs();
  setLEDs(0, 0, 100);
  //USB.println("LEDs set");
  delay(1000);
  //sendACK();
  //userLED_OFF();
	//beepOFF();
  //default to swapped XY and inverted X
  initialiseHapticUnit();
  initTft(1);
  clearPrintMessage("ETRT Perceptual Crossing", 1, 50, 1);
  printMessage("Ready . . .", 5, displayYCentre, 2);
  
  
  //setVib1Strength(0);
  delay(500);
  setLEDs(0, 0, 0);
  //startDisplayData();
  //updateDisplayData();
  
  clearPrintMessage("READY", tftXCursor, tftYCursor, tftTextSize);
}

void loop() {
  //updateLEDs();
  updateSystem();
}


void updateSystem(){
  //if(refreshSerial())packetsReceived++;
  //unsigned long oldpacketsReceived = packetsReceived;
  cmdDCU.update();
  //if(packetsReceived > oldpacketsReceived) beep(100);
  oldButtonState = buttonState;
  buttonState = checkSwitch();
  //if(buttonState) setLEDs(0, 0, 100);
  updateBeepFlash();
	if(!flashActive || !manualFlash) updateButton(); //only update the LED if a flash event is NOT running

  //if(!trialActive)updateManual();
  if(streamData) printDataStream();
  //if(displayData) updateDisplayData();
}

void updateManual(){
  //update for when not running a trial - copy button commands
  
  if(buttonState || flashActive) userLED_ON();
  else userLED_OFF();
  updateTft();
}
void updateButton(){
  if(!trialActive) return; //only update when running the trial
	if(buttonState)     userLED_ON();
  else                userLED_OFF();
}

void updateBeepFlash(){
  updateBeep();
	updateFlash();
}

void waitFor(unsigned long delayTime){
  unsigned long endTime = millis() + delayTime;
  //elapsedMillis delayTimer;
  while(millis() < endTime) updateSystem();
}

void startTrial(){
  //userLED_ON();
  //beepX(500);
  //userLED_OFF();
  trialActive = 1;
  hapticRate1 = 0;
  hapticRate2 = 0;
  dataSendTimer = millis();
  setHaptics();
  resetTrackerball();
  //beep(500);
}

void stopTrial(){
  //stopTrackerball();
  hapticRate1 = 0;
  hapticRate2 = 0;
  setHaptics();
  dataSendTimer = millis();
  trialActive = 0;
  //userLED_ON();
  //userLED_ON();
  //beepX(500);
  //userLED_OFF();
  

}

void setHaptics(){
  //outputs 0-1 are vibration Strength
  //outputs 2-3 and 4-5 are frequency and amplititude of analog output 0 and 1
  //vib0.setIntensity(device.outputs[0]);
  //vib1.setIntensity(device.outputs[1]);
  
  vib0.simpleSet( hapticRate1 );
  vib1.simpleSet( hapticRate2 );
  wave0.set(100.0, 0.0);
  wave1.set(100.0, 0.0);
}


/*
void setLEDs(int ledNum, uint8_t rd, uint8_t grn, uint8_t blu){
  leds.setPixelColor(ledNum, rd, grn, blu);
}*/
/*
void updateLED(){
  leds.show();
}*/
/*
void vibration1On(){
  digitalWrite(VIB1ENABLE, 1);
  digitalWrite(VIB1PWMPIN, 1);
}
void vibration1Off(){
  digitalWrite(VIB1ENABLE, 0);
  digitalWrite(VIB1PWMPIN, 0);
}

void vibration2On(){
  digitalWrite(VIB2ENABLE, 1);
  digitalWrite(VIB2PWMPIN, 1);
}
void vibration2Off(){
  digitalWrite(VIB2ENABLE, 0);
  digitalWrite(VIB2PWMPIN, 0);
}

void setVib1Strength(int intensity){
  if(intensity > 0){
    vibration1On();
    analogWrite(VIB1PWMPIN, intensity);
  }
  else {
    vibration1Off();
    analogWrite(VIB1PWMPIN, 0);
  }
}

void setVib2Strength(int intensity){
  if(intensity > 0){
    vibration2On();
    analogWrite(VIB2PWMPIN, intensity);
  }
  else {
    vibration2Off();
    analogWrite(VIB2PWMPIN, 0);
  }
}
void setHaptics(){
  
  setVib1Strength(hapticRate1);
  setVib2Strength(hapticRate2);
  //haptic1.setLra(hapticRate1);
  //haptic2.setLra(hapticRate2);
}*/
