//Serial port command handlers
//List of commands
//typedef bool (*commandHandler)(serialBuffer_t &); //command handler function pointer type
//Command handler array type - contains command string and function pointer
/*typedef struct commander_t{
  String commandString;
  commandHandler handler;
  String manualString;
} commander_t;*/

//An array of commands - each initialised with a command string and handler function
const commandList_t DCUCommands[] = {
  {"get status",        getDeviceStatus,    "get device status information"},
  {"set time",         setTime,            "set the time. Syntax: set time [HH]:[MM]:[SS]"},
  {"get time",          getTime,            "get the device time"},
  {"set date",         setDate,            "Set the date Syntax: set time [DD]:[MM]:[YYYY]"},
  {"get date",          getDate,            "Get the device date"},
  {"get raw voltage",   getRawV,            "Get the device raw voltage reading"},
  {"get settings",      getSettings,        "Print out the device settings"},
  {"set precision",     setDataPrecision,   "Sets the number of decimal places for data logging. Example: set precision 5"},
  {"set aux power",     set5V,              "Turn the auxiliary 5V power regulator on or off. Syntax: set aux power on"},
  {"sleep",             setSleep,           "Puts the device to sleep"},
  {"restart",           restartDCU,         "restarts the DCU"},
  {"set SSID",          setSSID,            "Sets the SSID for WiFI. Syntax: set SSID [WIFI NAME]"}, //setSSID0=
  {"set Pass",          setPassword,        "Sets the password for WiFI. Syntax: set Pass [WIFI PASS]"}, //setWiFiPass0=
  {"enable server",     enableServer,       "Starts the HTTP server"},
  {"disable server",    disableServer,      "Stope the HTTP server"},
  {"enable telnet",     enableTelnet,       "Starts the Telnet service"},
  {"disable telnet",    disableTelnet,      "Stops the Telnet service"},
  {"enable bluetooth",  enableBluetooth,    "Starts the Bluetooth service"},
  {"disable bluetooth", disableBluetooth,   "Stops the Bluetooth service"},
  {"enable UDP",        enableUDP,          "Starts the UDP service"},
  {"disable UDP",       disableUDP,         "Starts the UDP service"},
  {"startlog",          startLogging,       "Starts logging data"}, //start Logging Data
  {"stoplog",           stopLogging,        "Stops logging data"}, //stop Logging Data
  {"setlog",            setloglist,         "Configure which types of data are to be logged and where to send it (SD, USB, UDP, Bluetooth). Examples: setlog quaternion true  setlog USB true"}, //configure which data is to be logged
  {"get log header",    getLogHeader,       "Gets the header for the log file with names for each column of data"}, //return the log header
  {"SD",                sdFileHandler,      "Open SD explorer submenu"},
  {"start haptest",     startHapticTest,    "Start testing the haptics"}, //start Logging Data
  {"stop haptest",      stopHapticTest,     "Stop testing the haptics"}, //stop Logging Data
  {"set waveform",      setWaveform,        "Set the waveform type. Syntax:set waveform [channel] [TYPE]"}, //set the analog waveform type syntax = set waveform [channel] [TYPE]
  {"set gyro bias",     setBias,            "Set the Gyro bias values with three comma seperated integers. Syntax:set gyro bias [x],[y],[z]"},
  {"set accel bias",    setBias,            "Set the Accelerometer bias values with three comma seperated integers. Syntax:set gyro bias [x],[y],[z]"},
  {"set mag bias",      setBias,            "Set the magnetometer bias values with three comma seperated integers. Syntax:set gyro bias [x],[y],[z]"},
  {"ESP Boot",          espBootloaderMode,  "Set the ESP32 to bootloader mode"}, //go into bootloader and disable UART
  {"ESP RESET",         espReset,           "-Reset the ESP32"}, //restart ESP32 and UART
  {"ESP Status",        espStatusMessage,   "-An ESP32 WiFi module Status message"}, //Status message from ESP
  {"ESPGet",            espSendClientData,  "-Marks HTML data sent to the WiFI module for it to sent to a client"},  //get an HTTP page
  {"ESPTelnet",         espSendTelnetData,  "-Marks data as coming from a Telnet client"},  //
  {"ESPBT",             espSendBTData,      "-Marks data as coming from a Bluetooth connection"},  //
  {"ESP",               espSendCommand,     "Route the command to the ESP32 WiFi Module"}, //route a command to the ESP module  
  {"update esp",        updateEspHandler,   "Start the ESP32 OTA update service"},
  {"testSD",            testSD,             "Test the SD by opening, writing and closing a file"},
  {"button action",     buttonActionSet,    "Enable or disable various button actions"},
  {"print",             printSomethingHandler,    "Print a piece of data"},
  {"DEBUG",             debugMessage,       "-Debug message"},
  {"ack",               ack,                "-Acknowledge"},
  {"nack",              nack,               "-NOT Acknowledge"},
  {"debug",             setDebugPrinting,   "set debug options. Sub options are sd, usb or boot. Use 'on', 'off' or none to toggle. EG: 'debug on' or 'debug sd on'. 'debug usb on' causes device to wait for USB serial port before booting"},
  {"?",                 query,              "Query"},
  {"ERR",               commError,          "-comm Error"}
};

const uint16_t numOfDCUCmds = sizeof(DCUCommands);

//const uint16_t numOfCmds = sizeof(commands) /  sizeof(commands[0]); //calculate the number of commands so we know the array bounds
/*
bool commandFunctionTemplate( Commander &Cmdr ){

  return 0;
}
*/

//
/*
int getCommandEndIndex(Commander &Cmdr){
  //find the end of the characters in the command string - index AFTER the last character in the command string
  return commands[Cmdr.commandVal].commandString.length();
}*/

bool getTrueOrFalse(String dataString){
  if(dataString.indexOf("true") > -1) return true;
  return false;
}
bool printSomethingHandler(Commander &Cmdr){
  if(Cmdr.getPayloadString() == "sonar"){
    printSonar(Cmdr);
  }
}

bool commError(Commander &Cmdr){
  //Print to USB
  USBS.print("Comm error message: ");
  USBS.println(Cmdr.getPayload());
}

bool sdFileHandler(Commander &Cmdr){
  SDOK = !device.cardInitError;
  if(!SDOK){
    Cmdr.println("No SD Card");
    return 0;
  }
  //For the moment - block commands if they are chained
  if(Cmdr.hasPayload()){
    Cmdr.println("Chained commands are disabled");
    //This is blocking when using coolterm ...?
    return 0;
  }
  Cmdr.println("Opening SD Navigator");
  //Cmdr.attachCommands(getCommands, numOfGetCmds);
  cmdName = "SD:";
  if(Cmdr.transferTo(fileCommands, numOfFileCommands, cmdName)){
    //commander returns true if it is passing back control;
    Cmdr.transferBack(DCUCommands, numOfDCUCmds, prompt);
  }
  return 0;
}


//--------------------------------------------------------------------------------------------------------
bool buttonActionSet( Commander &Cmdr){
  //button action 
  /* Sub Commands:
   *  enable haptics
   */
  //int startOf = Cmdr.bufferString.indexOf("action");
  //startOf = Cmdr.bufferString.indexOf(" ", startOf); //find the space after the "button action " command
  //String subStr = Cmdr.bufferString.substring(startOf +1);
  bool isTrue = false;
  if(Cmdr.bufferString.indexOf(" true") > -1 || Cmdr.bufferString.indexOf(" True") > -1)isTrue = true; //getTrueOrFalse(subStr);

  //if(Cmdr.bufferString.indexOf("enable haptics") != -1)
  //parse substr
  if(Cmdr.bufferString.indexOf("enable haptics") != -1){
    device.buttonEnableHaptics = isTrue;
  }else if(Cmdr.bufferString.indexOf("enable log") != -1){
    device.buttonStartLog = isTrue;
  }else if(Cmdr.bufferString.indexOf("enable toggle") != -1){
    device.buttonActionToggle = isTrue;
  }
  if(Cmdr.getStreamType() == SERIAL_STREAM){
    Cmdr.println("Button action set");
  }
  return 0;
}

//--------------------------------------------------------------------------------------------------------
bool setBias( Commander &Cmdr){
  //extract three comma seperated values
  int biases[3] = {0,0,0};
  for(uint8_t n = 0; n < 3; n++){
    Cmdr.getInt(biases[n]);
  }

  /*
  int idx = Cmdr.bufferString.lastIndexOf(",");
  String tmp = Cmdr.bufferString.substring(idx+1);
  biases[0] = tmp.toInt();

  idx = Cmdr.bufferString.lastIndexOf(",", idx-1);
  tmp = Cmdr.bufferString.substring(idx+1);
  biases[1] = tmp.toInt();
  
  idx = Cmdr.bufferString.lastIndexOf(" ", idx-1);
  tmp = Cmdr.bufferString.substring(idx+1);
  biases[2] = tmp.toInt();*/
  uint8_t biasType = 0;
  //Find out which group of biases are being set
  if(Cmdr.bufferString.indexOf("gyro") > -1) biasType = 1;
  else if(Cmdr.bufferString.indexOf("accel") > -1) biasType = 2;
  else if(Cmdr.bufferString.indexOf("mag") > -1) biasType = 3;
  for(int n = 0; n < 3; n++) {
    if(biasType == 1)      settings.gyroBias[n] = biases[n];
    else if(biasType == 2) settings.accelBias[n] = biases[n];
    else if(biasType == 3) settings.magBias[n] = biases[n];
  }
  if(Cmdr.getStreamType() == SERIAL_STREAM){
    Cmdr.println("Biases set");
  }
  return 0;
}


bool setWaveform( Commander &Cmdr){
  String wavType = "";
  if(Cmdr.bufferString.indexOf("SINE")) wavType = "SINE";
  if(Cmdr.bufferString.indexOf("SQUARE")) wavType = "SQUARE";
  if(Cmdr.bufferString.indexOf("TRIANGLE")) wavType = "TRIANGLE";
  if(Cmdr.bufferString.indexOf("SAWF")) wavType = "SAWF";
  if(Cmdr.bufferString.indexOf("SAWB")) wavType = "SAWB";
  if(Cmdr.bufferString.indexOf("ONESHOT")) wavType = "ONESHOT";
  if(Cmdr.bufferString.indexOf("HALFSHOT")) wavType = "HALFSHOT";
  if(Cmdr.bufferString.indexOf("set waveform 0") > -1){
    //chanel 0
    setWaveType(0, wavType);
  }if(Cmdr.bufferString.indexOf("set waveform 1") > -1){
    //chanel 1
    setWaveType(1, wavType);
  }
  if(Cmdr.getStreamType() == SERIAL_STREAM){
    //print some messages
    Cmdr.println("Waveform Set");
  }
  return 0;
}


bool testSD( Commander &Cmdr){
  
  if(Cmdr.getStreamType() == SERIAL_STREAM){
    Cmdr.println("Running SD Test");
  }
  SDFileTest();
  return 0;
}

bool getSettings( Commander &Cmdr){
  printDeviceSettings(Cmdr);
  return 0;
}

/*
bool printSDDirectory( Commander &Cmdr){

  SD.ls(Cmdr.getOutputPort(), LS_R);
  //Cmdr.println
  return 0;
}*/
bool getLogHeader( Commander &Cmdr){
  Cmdr.println(getLogHeader());
  Cmdr.println(getLogColumns());
  return 0;
}
bool setloglist( Commander &Cmdr){
  bool bVal = false;
  if(Cmdr.bufferString.indexOf(" true") > -1 || Cmdr.bufferString.indexOf(" True") > -1) bVal = true;
    //bool bVal = Cmdr.containsTrue();
  if(Cmdr.getStreamType() == SERIAL_STREAM) Cmdr.print("Setting: ");
  //detect boolean values for different data types
  if(Cmdr.bufferString.indexOf("quaternion") > -1){
    device.logQ = bVal;
    if(Cmdr.getStreamType() == SERIAL_STREAM) Cmdr.print(" quaternion ");
  }
  if(Cmdr.bufferString.indexOf("YPR") > -1){
    device.logYPR = bVal;
    if(Cmdr.getStreamType() == SERIAL_STREAM) Cmdr.print(" YPR ");
  }
  if(Cmdr.bufferString.indexOf("gyro") > -1){
    device.logGyro = bVal;
    if(Cmdr.getStreamType() == SERIAL_STREAM) Cmdr.print(" gyro ");
  }
  if(Cmdr.bufferString.indexOf("accel") > -1){
    device.logAccel = bVal;
    if(Cmdr.getStreamType() == SERIAL_STREAM) Cmdr.print(" accel ");
  }
  if(Cmdr.bufferString.indexOf("mag") > -1){
    device.logMag = bVal;
    if(Cmdr.getStreamType() == SERIAL_STREAM) Cmdr.print(" mag ");
  }
  if(Cmdr.bufferString.indexOf("heading") > -1){
    device.logHeading = bVal;
    if(Cmdr.getStreamType() == SERIAL_STREAM) Cmdr.print(" heading ");
  }
  if(Cmdr.bufferString.indexOf("inputs") > -1){
    device.logInputs = bVal;
    if(Cmdr.getStreamType() == SERIAL_STREAM) Cmdr.print(" inputs ");
  }
  if(Cmdr.bufferString.indexOf("outputs") > -1){
    device.logOutputs = bVal;
    if(Cmdr.getStreamType() == SERIAL_STREAM) Cmdr.print(" outputs ");
  }
  //these select which outputs to log data to
  
  if(Cmdr.bufferString.indexOf("SD") > -1){
    device.logSD = bVal;
    if(Cmdr.getStreamType() == SERIAL_STREAM) Cmdr.print(" SD ");
  }
  if(Cmdr.bufferString.indexOf("UDP") > -1){
    device.logUDP = bVal;
    if(Cmdr.getStreamType() == SERIAL_STREAM) Cmdr.print(" UDP ");
  }
  if(Cmdr.bufferString.indexOf("bluetooth") > -1){
    device.logBluetooth = bVal;
    if(Cmdr.getStreamType() == SERIAL_STREAM) Cmdr.print(" bluetooth ");
  }
  if(Cmdr.bufferString.indexOf("telnet") > -1){
    device.logTelnet = bVal;
    if(Cmdr.getStreamType() == SERIAL_STREAM) Cmdr.print(" telnet ");
  }
  if(Cmdr.bufferString.indexOf("USB") > -1){
    device.logUSB = bVal;
    if(Cmdr.getStreamType() == SERIAL_STREAM) Cmdr.print(" USB ");
  }

  if(Cmdr.getStreamType() == SERIAL_STREAM){
    
    bVal ? Cmdr.println("true") : Cmdr.println("false");
    //Cmdr.println("Log set");
    printDeviceState(Cmdr);
  }
  return 0;
}
bool startHapticTest( Commander &Cmdr){
  if(Cmdr.getStreamType() == SERIAL_STREAM){
    Cmdr.println("Starting haptic test");
  }
  testHapticsStart();
  return 0;
}

bool stopHapticTest( Commander &Cmdr){
  testHapticsStop();
  if(Cmdr.getStreamType() == SERIAL_STREAM){
    Cmdr.println("Haptic test stopped");
  }
  return 0;
}
bool startLogging( Commander &Cmdr){
  if(Cmdr.getStreamType() == SERIAL_STREAM){
    Cmdr.println("Logging started");
  }
  startLog();
  return 0;
}
bool stopLogging( Commander &Cmdr){
  stopLog();
  if(Cmdr.getStreamType() == SERIAL_STREAM){
    Cmdr.println("Logging stoped");
  }
  return 0;
}
bool setDataPrecision( Commander &Cmdr){
  //uint8_t startOf = 0;
  //uint8_t endOf = 0;
  int result = 0;
  //set command - find the second space char
  //startOf = Cmdr.bufferString.indexOf("precision");
  //startOf = Cmdr.bufferString.indexOf(' ', startOf);
  //String subStr = Cmdr.bufferString.substring(startOf+1, Cmdr.bufferString.length());
  Cmdr.getInt(result); // = subStr.toInt();
  if(result > 0 && result < 8)  settings.dataPrecision = result;
  if(Cmdr.getStreamType() == SERIAL_STREAM){
    Cmdr.println("Data precision set");
  }
}
bool setSSID(Commander &Cmdr){
  if(DBG.STD) debugPrintln("Setting SSID");
  //uint8_t startOf = 0;
  //uint8_t endOf = 0;
  //set command - find the second space char
  //startOf = Cmdr.bufferString.indexOf("SSID");
  //startOf = Cmdr.bufferString.indexOf(' ', startOf);
  //endOf = Cmdr.bufferString.indexOf('\n');
  //SSID is after the space
  settings.ssid = Cmdr.getPayloadString();
  settings.wifiSSIDOK = true;
  if(DBG.STD){
    debugStartLine();
    DEBUG.print("Set SSID to: ");
    DEBUG.println(settings.ssid);
  }
  if(Cmdr.getStreamType() == SERIAL_STREAM){
    Cmdr.print("SSID set to ");
    Cmdr.println(settings.ssid);
  }
  return 0;
}

bool setPassword(Commander &Cmdr){
  if(DBG.STD) debugPrintln("Setting Password");
  //uint8_t startOf = 0;
  //uint8_t endOf = 0;
  //set command - find the second space char
  //startOf = Cmdr.bufferString.indexOf("Pass");
  //startOf = Cmdr.bufferString.indexOf(' ', startOf);
  //endOf = Cmdr.bufferString.indexOf('\n');
  //SSID is after the space
  settings.pswd = Cmdr.getPayloadString();
  settings.wifiPassOK = true;
  if(DBG.STD){
    debugStartLine();
    DEBUG.print("Set Password to: ");
    DEBUG.println(settings.pswd);
  }
  if(Cmdr.getStreamType() == SERIAL_STREAM){
    Cmdr.print("Password set to ");
    Cmdr.println(settings.pswd);
  }
  return 0;
}

bool enableServer(Commander &Cmdr){
  WIRELESS.println("enable server");
  if(DBG.STD) DEBUG.print("DEBUG: Sent Server start command");
  
  if(Cmdr.getStreamType() == SERIAL_STREAM)    Cmdr.println("Sent Server start command");
  settings.waitForReplyTimer = 1000;
  return 0;
}
bool disableServer(Commander &Cmdr){
  WIRELESS.println("disable server");
  if(DBG.STD) DEBUG.print("DEBUG: Sent Server stop command");
  if(Cmdr.getStreamType() == SERIAL_STREAM)    Cmdr.println("Sent Server stop command");
  settings.waitForReplyTimer = 1000;
  return 0;
}

bool enableTelnet(Commander &Cmdr){
  WIRELESS.println("enable telnet");
  if(DBG.STD) DEBUG.print("DEBUG: Sent telnet start command");
  if(Cmdr.getStreamType() == SERIAL_STREAM)    Cmdr.println("Sent telnet start command");
  settings.waitForReplyTimer = 1000;
  return 0;
}
bool disableTelnet(Commander &Cmdr){
  WIRELESS.println("disable telnet");
  if(DBG.STD) DEBUG.print("DEBUG: Sent telnet stop command");
  if(Cmdr.getStreamType() == SERIAL_STREAM)    Cmdr.println("Sent telnet stop command");
  settings.waitForReplyTimer = 1000;
  return 0;
}


bool enableBluetooth(Commander &Cmdr){
  WIRELESS.println("enable bluetooth");
  if(DBG.STD) DEBUG.print("DEBUG: Sent bluetooth start command");
  if(Cmdr.getStreamType() == SERIAL_STREAM)  Cmdr.println("Sent bluetooth start command");
  settings.waitForReplyTimer = 1000;
  return 0;
}
bool disableBluetooth(Commander &Cmdr){
  WIRELESS.println("disable bluetooth");
  if(DBG.STD) DEBUG.print("DEBUG: Sent bluetooth stop command");
  if(Cmdr.getStreamType() == SERIAL_STREAM) Cmdr.println("Sent bluetooth stop command");
  settings.waitForReplyTimer = 1000;
  return 0;
}

bool enableUDP(Commander &Cmdr){
  WIRELESS.println("enable UDP");
  if(DBG.STD) DEBUG.print("DEBUG: Sent UDP start command");
  if(Cmdr.getStreamType() == SERIAL_STREAM)  Cmdr.println("Sent UDP start command");
  settings.waitForReplyTimer = 1000;
  return 0;
}
bool disableUDP(Commander &Cmdr){
  WIRELESS.println("disable UDP");
  if(DBG.STD) DEBUG.print("DEBUG: Sent UDP stop command");
  if(Cmdr.getStreamType() == SERIAL_STREAM)  Cmdr.println("Sent UDP stop command");
  settings.waitForReplyTimer = 1000;
  return 0;
}

bool setTime(Commander &Cmdr){
  //DEBUG.println("Setting the RTC time . . .");
  int res = 0;

  if(Cmdr.getInt(res)) Clock.setHours((uint8_t)res);// = subStr.toInt();
  if(Cmdr.getInt(res)) Clock.setMinutes((uint8_t)res);// = subStr.toInt();
  if(Cmdr.getInt(res)) Clock.setSeconds((uint8_t)res);// = subStr.toInt();

  //reset the millisecond timer
  device.clockLastMillis = millis();
  delay(10); //for some reason the readback is off - perhaps he RTC takes too long to sync and the old seconds are printed
  //Clock.setTime(hrs, mins, secs);
  getTime(Cmdr);
  return 0;
}

bool getTime(Commander &Cmdr){
  Cmdr.print("Time=");
  Cmdr.println(getTimestamp());
}


bool setDate(Commander &Cmdr){
  int res = 0;
  //DEBUG.println("Setting the RTC date . . .");
  if(Cmdr.getInt(res)) Clock.setDay((uint8_t)res);// = subStr.toInt();
  if(Cmdr.getInt(res)) Clock.setMonth((uint8_t)res);// = subStr.toInt();
  if(Cmdr.getInt(res)) Clock.setYear((uint8_t)res);// = subStr.toInt();
  delay(10); //for some reason the readback is off - perhaps he RTC takes too long to sync and the old year is printed
  getDate(Cmdr);
  return 0;
}

bool getDate(Commander &Cmdr){
  Cmdr.print("Date=");
  Cmdr.println(getDate());
  return 0;
}

bool getRawV(Commander &Cmdr){
  Cmdr.print("Raw Voltage=");
  Cmdr.println(analogRead(BATTERYMON));
  return 0;
}


bool set5V(Commander &Cmdr){
  Cmdr.bufferString.toLowerCase();
  if(Cmdr.bufferString.indexOf("on") > -1){
    enable5VPower();
    //Cmdr.println("5V ON");
  }
  else if(Cmdr.bufferString.indexOf("off") > -1){
    disable5VPower();
    //Cmdr.println("5V OFF");
  }
  
  if(Cmdr.getStreamType() == SERIAL_STREAM) {
    Cmdr.print("5V");
    device.v5PowerOn ? Cmdr.print("on") : Cmdr.print("off");
  }
  return 0;
}

bool setSleep(Commander &Cmdr){
  //shut down peripherals
  sleepDevice();
  
  return 0;
}

bool restartDCU(Commander &Cmdr){
  //shut down peripherals
  restartDevice();
  
  return 0;
}
bool getDeviceStatus(Commander &Cmdr){
  printDeviceState(Cmdr);
  return 0;
}

bool debugMessage(Commander &Cmdr){
  if(settings.printDebugMessages){
    Cmdr.println(Cmdr.bufferString);
  }
  return 0;
}

bool ack(Commander &Cmdr){
  DEBUG.println("ACK received");
  return 0;
}
bool nack(Commander &Cmdr){
  DEBUG.println("NACK received");
  return 0;
}


bool espReset(Commander &Cmdr){
  /*
   * Reset the ESP32
   */
   Cmdr.println("DCU Status: Resetting ESP32");
   startESPApplication();
   
   //disableESP();
   //disableESPBootloader();
   //delay(1000); //wait a sec ...?
   //Cmdr.println("Done, starting ESP32");
   //Cmdr.flush();
   //delay(100);
   //settings.ESPBootMode = false;
   //enableESP();
   return 0;
}
bool espBootloaderMode(Commander &Cmdr){
  /*
   * Setup the ESP for bootloader mode
   * Set bootloader pin state then disable and re-enable
   * Set the USB Serial port to pass data straight to the ESP
   * Set the ESP Serial to pass data straight back
   */
   Cmdr.println("DCU Status: Configuring ESP32 for booloader mode");
   startESPBootloader();
   return 0;
}
bool query(Commander &Cmdr){
  Cmdr.print("DCU Status: ETRT DCU Firmware Version ");
  Cmdr.println(settings.DCU_FW_VERSION);
  //printCommandList(Cmdr);
  printDeviceState(Cmdr);
  return 0;
}
/*
bool help(Commander &Cmdr){
  //ERRORS.println("Command List:");
  //Cmdr.println("Command List:");
  printCommandList(Cmdr);
  return 0;
}*/

bool setDebugPrinting(Commander &Cmdr){
  //ERRORS.println("Serial handler 2");
  Cmdr.println("DCU Status: Setting debug printing");
  if(Cmdr.bufferString.indexOf("boot") > -1){
    if(Cmdr.bufferString.indexOf(" on")) DBG.BOOT = true;
    else if(Cmdr.bufferString.indexOf(" off")) DBG.BOOT = false;
    else DBG.BOOT = !DBG.BOOT;
    DBG.BOOT ? Cmdr.println("Debug boot ON") : Cmdr.println("Debug boot OFF");
    return 0;
  }
  if(Cmdr.bufferString.indexOf("sd") > -1){
    if(Cmdr.bufferString.indexOf(" on")) DBG.SDC = true;
    else if(Cmdr.bufferString.indexOf(" off") > -1) DBG.SDC = false;
    else DBG.SDC = !DBG.SDC;
    DBG.SDC ? Cmdr.println("Debug sd ON") : Cmdr.println("Debug sd OFF");
    return 0;
  }
  if(Cmdr.bufferString.indexOf("usb") > -1){
    if(Cmdr.bufferString.indexOf(" on") > -1) DBG.WAIT_USB = true;
    else if(Cmdr.bufferString.indexOf(" off") > -1) DBG.WAIT_USB = false;
    else DBG.WAIT_USB = !DBG.WAIT_USB;
    DBG.WAIT_USB ? Cmdr.println("Debug USB Pause ON") : Cmdr.println("Debug USB Pause OFF");
    return 0;
  }
  if(Cmdr.bufferString.indexOf("save") > -1){
    saveDebugMode();
    Cmdr.println("Debug state saved to flash");
    return 0;
  }
  if(Cmdr.bufferString.indexOf(" on")) DBG.STD = true;
  else if(Cmdr.bufferString.indexOf(" off")) DBG.STD = false;
  else DBG.STD = !DBG.STD;
  DBG.STD ? Cmdr.println("Standard debug ON") : Cmdr.println("Standard debug OFF");
  return 0;
}


bool espStatusMessage(Commander &Cmdr){
  //look through the string to see what if any status message came in
  /*if(DBG.STD){
    DEBUG.print("DEBUG: ESP Status Message is: ");
    DEBUG.println(Cmdr.bufferString);
  }*/
  if(!Cmdr.hasPayload()) return 0;
  String messagePayload = Cmdr.getPayloadString();
  if(messagePayload.indexOf("ONLINE") > -1)                  device.espOnline = true;
  else if(messagePayload.indexOf("UDP Enabled") > -1)        device.espUDPEnabled = true;
  else if(messagePayload.indexOf("UDP Disabled") > -1)       device.espUDPEnabled = false;
  else if(messagePayload.indexOf("Server Enabled") > -1)     device.espServerEnabled = true;
  else if(messagePayload.indexOf("Server Disabled") > -1)    device.espServerEnabled = false;
  else if(messagePayload.indexOf("Telnet Enabled") > -1)     device.espTelnetEnabled = true;
  else if(messagePayload.indexOf("Telnet Disabled") > -1)    device.espTelnetEnabled = false;
  else if(messagePayload.indexOf("Bluetooth Enabled") > -1)  device.espBTEnabled = true;
  else if(messagePayload.indexOf("Bluetooth Disabled") > -1) device.espBTEnabled = false;
  else if(messagePayload.indexOf("BlueTooth=") > -1){
    device.BTName = messagePayload.substring(Cmdr.bufferString.indexOf("=")+1);
  }
  else if(messagePayload.indexOf("WiFiIP") > -1){
    device.espWiFiConnected = true;
    uint8_t startOf = messagePayload.indexOf('=');
    device.localIPAddress = messagePayload.substring(startOf+1);
    //Serial.print("Connected. IP Address="); Serial.println(device.localIPAddress);
  }
  else if(messagePayload.indexOf("Version") > -1){
    device.espWiFiConnected = true;
    uint8_t startOf = messagePayload.indexOf("Version");
    device.espFWVersion = messagePayload.substring(startOf+8);
    //Serial.print("Connected. IP Address="); Serial.println(device.localIPAddress);
  }
  else if(messagePayload.indexOf("WiFiConnErr") > -1){
    device.espWiFiConnected = false;
    device.espWiFiConnectionErrors++;
  }//else if(Cmdr.bufferString.indexOf("ESP Status: Version" > -1){
    //Serial.print(Cmdr.getPayload());
  //}
  Serial.print(Cmdr.getPayload());
  //DEBUG.println(Cmdr.bufferString);
  return 0;
}
bool espSendClientData(Commander &Cmdr){
  //Check the string to see what the client asked for then stream data back
  //Cmdr.print("serverSend "); //send the command
  //Is it a command
  if(Cmdr.bufferString.indexOf("Command") > -1){
    processServerCommandRequest(Cmdr);
  }
  return 0;
}

bool espSendTelnetData(Commander &Cmdr){
  //Set the prefix so all replies are appended with telnetSend
  
  if(Cmdr.hasPayload()){
    Cmdr.setPrefix("telnetSend ");
    //Cmdr.startFormatting();
    Cmdr.feedString(Cmdr.getPayload());
  }
  return 0;
  //Cmdr.println("telnetSend OK");
  //return 0;
}
bool espSendBTData(Commander &Cmdr){
  //Set the prefix so all replies are appended with BTSend
  
  if(Cmdr.hasPayload()){
    Cmdr.setPrefix("BTSend ");
    //Cmdr.startFormatting();
    Cmdr.feedString(Cmdr.getPayload());
  }
  return 0;
  //Cmdr.println("telnetSend OK");
  //return 0;
}
bool espSendCommand(Commander &Cmdr){
  //Route a command to the ESP from source (USB or SD Card or even bluetooth?
  //Strip out the "ESP:" prefix and send to ESP
  //int startIndex = Cmdr.bufferString.indexOf(':');
  //String espCmd = Cmdr.bufferString.substring(startIndex);
  WIRELESS.print(Cmdr.getPayload());
  //WIRELESS.print(Cmdr.bufferString.substring(startIndex+1)); //the substring will include the newline
  if(DBG.STD){
    debugStartLine();
    DEBUG.print("Sent ESP: ");
    DEBUG.println(Cmdr.getPayload());
  }
  return 0;
}

bool updateEspHandler(Commander &Cmdr){
  if(Cmdr.getStreamType() == SERIAL_STREAM) {
    if(!device.espWiFiConnected){
      Cmdr.println("Error, no wifi");
      return 0;
    }
    Cmdr.println("Starting ESP OTA update service");
    Cmdr.print("IP Address="); Cmdr.println(device.localIPAddress);
    WIRELESS.println("start OTA");
  }
  return 0;
}/*
bool unknownCommand(Commander &Cmdr){
  //ERRORS.println("Unknown Serial Command");
  Cmdr.println("Unknown Serial Command");
  return 0;
}*/
bool commentCommand(Commander &Cmdr){
  //ERRORS.println("Unknown Serial Command");
  Cmdr.println("Command Comment");
  return 0;
}
