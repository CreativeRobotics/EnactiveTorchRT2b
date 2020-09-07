//DCU HW Revision b
#include "ETRT-DCU.h"
#include "haptics.h"
#include "src/Drivers/TOFLidarSensor.h"
#include "src/Drivers/SonarSensor.h"
//#include "src/Drivers/EZSonar.h"
//#include "Drivers/GPHaptics.h"
TOFLidarSensor Sensor;
SonarSensor Sonar;



Adafruit_NeoPixel leds(1, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);
int testVar;
int delayTime = 10;
int randomVar = 0;
bool ledState = HIGH;

uint8_t ledR = 0, ledG = 0, ledB = 0;
//bool forceESPBoot = false;

int sonarInts;

extern const uint16_t       numOfDCUCmds; //This is a forward declarationso the compiler knows we are going to declare this variable properly later
extern const commandList_t  DCUCommands[]; //forward declare the master command list
//extern const uint16_t       numOfESPCmds; //This is a forward declarationso the compiler knows we are going to declare this variable properly later
//extern const commandList_t  espCommands[]; //forward declare the master command list


void setup() {
  // put your setup code here, to run once:
  //pinMode(PB12, OUTPUT);
  //Start here then the main loop is a state machine
  
  //For some reason these need to be here now or it will crash the MCU
  attachInterrupt(digitalPinToInterrupt(PIN_IMU_INT), imu_isr, FALLING);
  attachInterrupt(digitalPinToInterrupt(SDCD), cardDetect_isr, CHANGE);

  //SONAR:
  
  pinMode(pulsePin, INPUT);
  if(device.sonarMode != SONAR_SERIAL_MODE){
    attachInterrupt(digitalPinToInterrupt(pulsePin), sonarPulsePin_isr, CHANGE);
  }
  //setup serial ports and commander
  USBS.begin(115200);
  WIRELESS.begin(115200);
  //while(!USBS){;}
  
  if(DBG.BOOT) Serial.println("State: Starting up");
  device.deviceState = runStartup();
  if(DBG.BOOT) {
    Serial.println("State: Startup state ended - curr state:");
    printDeviceState(cmdDCU);
  }
  setTopLayer(DCUCommands, numOfDCUCmds, prompt);
  //Attach the predefined special handler - this is required for writing files
  cmdDCU.attachSpecialHandler(streamToFileHandler);
  cmdDCU.begin(&USBS, DCUCommands, numOfDCUCmds);
  cmdDCU.commanderName = "ETRT>";
  cmdDCU.commandPrompt(ON);
  cmdWireless.begin(&WIRELESS, DCUCommands, numOfDCUCmds);
  
  cmdWireless.attachAltPort(&USBS);
  //cmdWireless.enablePipe();
  
  //cmdDCU.attachAltPort(&WIRELESS);
  cmdWireless.echoToAlt(false);
  enablePeripheralPower();
  setLEDs(redLED);
}

byte secnd = 0;
unsigned int loopCount = 0;

void loop() {
  //This is a state machine
  /*
   * The device runs the initial startup code in setup()
   * and then goes to the state machine where it runs through a startup sequence
   */
  switch(device.deviceState){
    case DCU_SPEEPING:
      if(DBG.BOOT) Serial.println("State: Sleeping");
      //do nothing
      //If you wake up you will be in this state
      
      setSleepInt(0);
      device.deviceState = runStartup();  //Call this until it changes the state
        //device.deviceState = DCU_FINISHED_STARTUP;
        //device.deviceState = DCU_STARTING_SDHC;
      enablePeripheralPower();
      break;
    case DCU_GOINGTOSLEEP:
      //Set clock to sleep mode
      if(DBG.BOOT) Serial.println("State: Going to sleep");
      device.deviceState = DCU_SPEEPING;
      Clock.standbyMode();
      break;
    case DCU_STARTING_SDHC:
      //setLEDs(redLED);
      if(DBG.BOOT) Serial.println("State: Starting SDHC");
      device.deviceState = startSDHC();  //Call this until it changes the state
      break;
    case DCU_STARTING_WIRELESS:
      //setLEDs(redLED);
      if(DBG.BOOT) Serial.println("State: Starting Wireless");
      if(device.printStartupState) printOnce("DCU State: Starting ESP32");
      device.deviceState = startWireless();
      updateComms();
      //Start the ESP32
      break;
    case DCU_STARTING_OUTPUTS:
      if(DBG.BOOT) Serial.println("State: Starting Outputs");
      //if(device.printStartupState) printOnce("DCU State: Starting Output Module");
      enable5VPower();
      //setLEDs(redLED);
      device.deviceState = startOutputs();
      updateComms();
      break;
    case DCU_STARTING_INPUTS:
      if(DBG.BOOT) Serial.println("State: Starting Inputs");
      //if(device.printStartupState) printOnce("DCU State: Starting Input Module");
      //setLEDs(redLED);
      device.deviceState = startInputs();
      updateComms();
      break;
    case DCU_START_READING_SETTINGS:
      if(DBG.BOOT) Serial.println("State: Starting to read settings");
      //if(device.printStartupState) printOnce("DCU State: Reading Settings File");
      //setLEDs(greenLED);
      device.deviceState = startReadingSettings();  //Start reading the settings file
      break;
    case DCU_READING_SETTINGS:
      if(DBG.BOOT) Serial.println("State: Reading settings");
      //setLEDs(greenLED);
      device.deviceState = readSettings();  //Call this until it changes the state
      break;
    case DCU_APPLYING_DEFAULTS:
      if(DBG.BOOT) Serial.println("State: Applying defaults");
      //setLEDs(redLED);
      //if(device.printStartupState) printOnce("DCU State: Apply default settings");
      device.deviceState = applyDefaults();
      updateComms();
      break;
    case DCU_STARTING_IMU:
      if(DBG.BOOT) Serial.println("State: Starting IMU");
      //setLEDs(redLED);
      //if(device.printStartupState) printOnce("DCU State: Starting IMU");
      device.deviceState = startIMU();
      updateComms();
      //Start the MPU9250
      break;
    case DCU_CONNECTING_WIFI:
      if(DBG.BOOT) Serial.println("State: Connecting to WiFi");
      //setLEDs(redLED);
      //if(device.printStartupState) printOnce("DCU State: Connecting to WiFI");
      device.deviceState = connectWiFi();
      updateComms();
      //Attempt a connection - send the wifi ssid and password to the ESP32
      break;
    case DCU_WAITING_FOR_WIFI:
      if(DBG.BOOT) Serial.println("State: Waiting for WiFi");
      //setLEDs(redLED);
      //if(device.printStartupState) printOnce("DCU State: Waiting For WiFI");
      device.deviceState = waitForWiFi();
      updateComms();
      //Wait for ESP32 to connect
      break;
    case DCU_RESTARTING_SDHC:
      if(DBG.BOOT) Serial.println("State: Restarting SDHC");
      //setLEDs(redLED);
      //if(device.printStartupState) printOnce("DCU State: Restarting SDHC");
      device.deviceState = restartSDHC();
      updateComms();
      //Wait for ESP32 to connect
      break;
    case DCU_FINISHED_STARTUP:
      if(DBG.BOOT) Serial.println("State: Finished Startup");
      //randomiseLED(randomLED);
      //setLEDs(randomLED);
      //beepFor(500); //CRASHES USB!!!!
      enableAudio();
      setLEDs(whiteLED);
      //SDFileTest();
      if(device.printStartupState) printOnce("DCU State: Finished - ready to go");
      device.deviceState = DCU_RUNNING_STANDBY;
      updateComms();
      //Wait for ESP32 to connect
      break;
    case DCU_RUNNING_STANDBY:
      //setLEDs(blueLED);
      if(DBG.BOOT) printOnce("DCU State: Running Standby");
      device.deviceState = updateIdle();
      break;
    case DCU_RUNNING_EXPERIMENT:
      //setLEDs(whiteLED);
      if(DBG.BOOT) printOnce("DCU State: Running Experiment");
      device.deviceState = updateExperiment();
      break;
    default:
    
      if(DBG.BOOT) Serial.println("State: ERRo DEFAULT STATE HANDLER REACHED");
      //setLEDs(redLED);
      device.deviceState = updateIdle();
      break;
  }
}

bool updateNow(){ //returns true if it is time for a log update
  if(millis() < device.nextUpdate) return false;
  device.nextUpdate = millis() + device.updateInterval;
  return true;
}

//UPDATE ALL THE THINGS ===============================================================================================
void updateSystem(){
  //updateBatteryVoltage();
  refreshUserControls();
  updateComms();
  updateIO();
  //checkForOffSwitch(); //check to see if the device is switched off
  updateBatteryVoltage();
  //Print every second
  if(secnd != Clock.getSeconds()){
    secnd = Clock.getSeconds();
    secondTicker();
    //testSDCard();
  }
}

void updateComms(){

    cmdDCU.update();
    cmdWireless.update();
    //update again to handle any pending commands
    cmdDCU.update();
    cmdWireless.update();
    //cmdDCU.updatePending();
    //cmdWireless.updatePending();
    updateIMU();

}

//===============================================================================================
void printOnce(String printMessage){
  static String lastMsg = "";
  if(lastMsg == printMessage) return;
  lastMsg = printMessage;
  DEBUG.println(lastMsg);
}

void testSDCard(){
  if(!device.dataFileOpen) return;
  DEBUG.println(millis());
  createLogLine(); //creates a formatted String of data for the log
  
  updateDataLogs(); //writes the string of data to anything configured to accept it
  DEBUG.println(millis());
}

void testHapticsStart(){
  
  beepOn();
  enableAudio();
  wave0.set(100.0, 1.0);
  wave1.set(100.0, 1.0);
  //vib0.setIntensity(0.8);
  //vib1.setIntensity(0.8);;
  vib0.simpleSet(1020);
  vib1.simpleSet(1020);
}
void testHapticsStop(){
  
  beepOff();
  wave0.set(100.0, 0.0);
  wave1.set(100.0, 0.0);
  //vib0.setIntensity(0.0);
  //vib1.setIntensity(0.0);
  vib0.simpleSet(0);
  vib1.simpleSet(0);
  disableAudio();
}
void secondTicker(){
  loopCount++;
}
