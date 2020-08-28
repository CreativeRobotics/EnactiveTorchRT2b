//connect to VM4789902 vrtqvfDxr8hq
//connect to AndroidAP hvpm2495

//bool query(serialBuffer_t &sBuff);
//bool help(serialBuffer_t &sBuff);

const commandList_t ESPCommands[] = {
  {"cmd test",          testCommander,        "run diagnostic on cmd"},
  {"?",                 query,                "Query"},
  {"ack",               ack,                  "ack"},
  {"nack",              nack,                 "nack"},
  {"set debug",         setDebugState,        "set debugging state"},
  {"get status",        printStatus,          "get device status"},
  {"get network time",  getTime,              "get network time"},
  {"set time server",  setTimeServer,        "set the time server URL"},
  {"set gmt offset",   setGMTOffset,         "set the GMT time offset"},
  {"enable server",     startEnableServer,    "enable web server"},
  {"disable server",    disableServer,        "disable web server"},
  {"enable telnet",     startEnableTelnet,    "enable telnet server"},
  {"disable telnet",    disableTelnet,        "disable telnet server"},
  {"enable bluetooth",  startEnablebluetooth, "enable BT"},
  {"disable bluetooth", disablebluetooth,     "disable BT"},
  {"enable UDP",        startEnableUDP,       "enable UDP"},
  {"disable UDP",       disableUDP,           "disable UDP"},
  {"UDPSend:",          udpSendData,          "send UDP line"},
  {"serverSend:",       serverSendLine,       "send webserver line"},
  {"telnetSend:",       telnetSendLine,       "send telnet line"},
  {"BTSend:",           btSendLine,           "sent BT line"},
  {"serverEnd",         serverEndLine,        "end server page"},
  {"set UDPPort",       setUDPPort,           "set UDP port"},
  {"set UDPAddress",    setUDPAddress,        "set UDP address"},
  {"set SSID",          setSSID,              "set WiFI SSID"}, //setSSID0=
  {"set Pass",          setPassword,          "set WiFi password"}, //setWiFiPass0=
  {"set Server Timeout",setServerTimeout,     "Server response timeout period"}, //setWiFiPass0=
  {"set BTName",        setBluetoothName,     "set the BT name"}, //setWiFiPass0=
  {"connect to",        wifiConnectTo,        "connect to WiFI (SSID) (Password)"},
  {"set WiFi Timeout",  setWiFiTimeout,       "set WiFi connect timeout"}, //setWiFiPass0=
  {"disconnect",        wifiDisconnect,       "disconnect WiFi"},
  {"start OTA",         startOTA,             "start the OTA update service"}
};
const uint16_t numOfESPCmds = sizeof(ESPCommands);// /  sizeof(ESPCommands[0]); //calculate the number of commands so we know the array bounds

//Serial port command handlers ======================================================================================================
//startEnableTelnet
//disableTelnet
//telnetSendLine


bool testCommander(Commander &Cmdr){
  Cmdr.printDiagnostics();
  Cmdr.print("ESPCmds ");
  Cmdr.println(numOfESPCmds);
  Cmdr.print("CmdArr Size ");
  Cmdr.println(sizeof(ESPCommands));
  Cmdr.print("CmdArr Item Size ");
  Cmdr.println(sizeof(ESPCommands[0]));
}

bool startOTA(Commander &Cmdr){
  if(device.serverEnabled || device.btEnabled || device.udpEnabled){
    Cmdr.println("ESP Status: OTA start failed. Services already running");
    return 0;
  }
  if(!device.wifiConnected){
    Cmdr.println("ESP Status: OTA start failed. WiFi not connected");
    return 0;
  }
  device.startOTA = true;
  //device.startServer = true;
  //the server will now start . . .
  return 0;
}

bool setDebugState(Commander &Cmdr){
  //DEBUG.println("Enabled Server");
  if(Cmdr.bufferString.indexOf("on") > -1){
    DEBUGGING = true;
    Cmdr.println("ESP Status: Debug messages on");
  }else{
    DEBUGGING = false;
    Cmdr.println("ESP Status: Debug messages off");
  }
  return 0;
}
//======================================================================================================
bool telnetSendLine(Commander &Cmdr){
  if(DEBUGGING) debugPrintln("Sending to telnet client");
  //uint8_t startOf = 0, endOf = 0;
  //find a space
  //startOf = Cmdr.bufferString.indexOf(':');
  //endOf = Cmdr.bufferString.length();
  if( device.telnetEnabled ){
    sendToTelnet( Cmdr.getPayloadString() ); //strip any newlines
    //sendToTelnet(Cmdr.bufferString.substring(startOf+1, endOf-1)); //strip any newlines
  }
  if(DEBUGGING) debugPrintln(Cmdr.getPayloadString());//Cmdr.bufferString.substring(startOf+1));
  return 0;
}

//======================================================================================================
bool startEnableTelnet(Commander &Cmdr){
  //DEBUG.println("Enabled Server");
  //Cmdr.println("ESP: Server Enabled");
  device.startTelnet = true;
  return 0;
}

bool disableTelnet(Commander &Cmdr){
  //DEBUG.println("Enabled Server");
  Cmdr.println("ESP Status: Telnet Disabled");
  device.startTelnet = false;
  disableTelnet();
  return 0;
}



bool printStatus(Commander &Cmdr){
  //print out the device state
  //Cmdr.println("ESP ETRT OTA Firmware");
    if(device.wifiConnected) {
    Cmdr.println("ESP Status: WiFi Connected");
    delay(10);
    Cmdr.print("ESP Status: WiFiIP=");
    Cmdr.println(WiFi.localIP());
  }else Cmdr.println("ESP Status: WiFi Disconnected");
  delay(10);
  
  if(device.serverEnabled)  Cmdr.println("ESP Status: Server Enabled");
  else                      Cmdr.println("ESP Status: Server Disabled");
  delay(10);
  if(device.udpEnabled)     Cmdr.println("ESP Status: UDP Enabled");
  else                      Cmdr.println("ESP Status: UDP Disabled");
  delay(10);
  if(device.btEnabled)      Cmdr.println("ESP Status: BT Enabled");
  else                      Cmdr.println("ESP Status: BT Disabled");
}


bool setBluetoothName(Commander &Cmdr){
  //uint8_t startOf = 0;
  //uint8_t endOf = 0;
  //set command - find the second space char
  //startOf = Cmdr.bufferString.indexOf(' ');
  //startOf = Cmdr.bufferString.indexOf(' ', startOf+1);
  //endOf = Cmdr.bufferString.length();
  //String substr = Cmdr.bufferString.substring(startOf+1, endOf);
  //ignore if there is only one character
  if(Cmdr.hasPayload()) device.btName = Cmdr.getPayloadString();
  //if(substr.length() > 1)  device.btName = substr;
  return 0;
}

bool setServerTimeout(Commander &Cmdr){
  if( Cmdr.getInt(device.clientConnectionTimeout) ){
    if(device.clientConnectionTimeout < 10) device.clientConnectionTimeout = 10;
  }
  /*
  int tmout = 10;
  if( Cmdr.getInt(tmout) ){
    if(tmout < 10) device.clientConnectionTimeout = 10;
    else device.clientConnectionTimeout = (unsigned long)tmout;
  }*/
  /*
  uint8_t startOf = 0;
  uint8_t endOf = 0;
  int val = 0;
  //set command - find the final space char
  startOf = Cmdr.bufferString.indexOf("Timeout");
  startOf = Cmdr.bufferString.indexOf(' ', startOf);
  endOf = Cmdr.bufferString.length();
  String substr = Cmdr.bufferString.substring(startOf+1, endOf);
  val = substr.toInt();
  //Don't set it to less than 10ms
  if(val < 10) device.clientConnectionTimeout = 10;
  else device.clientConnectionTimeout = val;*/
  return 0;
}

bool setWiFiTimeout(Commander &Cmdr){
  if( Cmdr.getInt(device.wifiConnectTimeout) ){
    if(device.wifiConnectTimeout < 10) device.wifiConnectTimeout = 10;
  }
  /*
  uint8_t startOf = 0;
  uint8_t endOf = 0;
  int val = 0;
  //set command - find the final space char
  startOf = Cmdr.bufferString.indexOf("Timeout");
  startOf = Cmdr.bufferString.indexOf(' ', startOf);
  endOf = Cmdr.bufferString.length();
  String substr = Cmdr.bufferString.substring(startOf+1, endOf);
  val = substr.toInt();
  //Don't set it to less than 1000 ms
  if(val < 1000) device.wifiConnectTimeout = 1000;
  else device.wifiConnectTimeout = val;*/
  return 0;
}



bool wifiConnectTo(Commander &Cmdr){
  if(DEBUGGING) debugPrintln("Trying to connect to ...");

  uint8_t startOf = 0;
  uint8_t endOf = 0;

  String payload = Cmdr.getPayloadString();
  if(DEBUGGING){
    Cmdr.print("Payload string = ");
    Cmdr.println(Cmdr.getPayloadString());
  }
  endOf = payload.indexOf(' ');
  ctSSID = payload.substring(0, endOf);
  ctPswd = payload.substring(endOf+1);
  /*
  //set command - find the second space char
  startOf = Cmdr.bufferString.indexOf(' ');
  startOf = Cmdr.bufferString.indexOf(' ', startOf+1);
  endOf = Cmdr.bufferString.indexOf(' ', startOf+1);
  ctSSID = Cmdr.bufferString.substring(startOf+1,endOf);
  startOf = endOf+1;
  endOf = Cmdr.bufferString.length();//Cmdr.bufferString.indexOf('\n');
  ctPswd = Cmdr.bufferString.substring(startOf, endOf);*/

  if(DEBUGGING) {debugPrint("SSID=["); DEBUG.print(ctSSID); DEBUG.println(']');}
  if(DEBUGGING) {debugPrint("PSWD=["); DEBUG.print(ctPswd); DEBUG.println(']');}
  //wifiLogins[WIFI_LIST-1].ssid = ctSSID;
  //wifiLogins[WIFI_LIST-1].pswd = ctPswd;
  //device.wifiListIndex = WIFI_LIST-1;
  device.connectToWiFi = true;
  return 0;
}

bool wifiDisconnect(Commander &Cmdr){
  disconnectWiFi();
  device.wifiConnected = false;
  Cmdr.println("ESP Status: WiFi Disconnected");
}


bool setSSID(Commander &Cmdr){
  if(DEBUGGING) debugPrintln("Setting SSID");
  if(Cmdr.hasPayload()){
    ctSSID = Cmdr.getPayloadString();
  }
  if(DEBUGGING){
    debugStartLine();
    DEBUG.print("Set SSID to: ");
    DEBUG.println(ctSSID);
  }
  return 0;
}

bool setPassword(Commander &Cmdr){
  if(DEBUGGING) debugPrintln("Setting Password");
  if(Cmdr.hasPayload()){
    ctPswd = Cmdr.getPayloadString();
  }
  if(DEBUGGING){
    debugStartLine();
    Cmdr.print("Set Password to: ");
    Cmdr.println(ctPswd);
  }
  return 0;
}
bool query(Commander &Cmdr){
  Cmdr.println("ESP Status: ESP32 OK");
  //Cmdr.println(settings.DCU_FW_VERSION);
  //printCommandList(sBuff);
  //printDeviceState(sBuff);
  return 0;
}

bool setTimeServer(Commander &Cmdr){
  if(DEBUGGING) debugPrintln("Setting SSID");
  /*uint8_t startOf = 0;
  uint8_t endOf = 0;
  int item = -1;
  //set command - find the second space char
  startOf = Cmdr.bufferString.indexOf("server");
  startOf = Cmdr.bufferString.indexOf(' ', startOf);
  endOf = Cmdr.bufferString.length();
  String tsText = Cmdr.bufferString.substring(startOf+1, endOf);*/
  if(Cmdr.hasPayload()) device.timeServer = Cmdr.getPayloadString();//substring(startOf-1);
  if(DEBUGGING){
    debugStartLine();
    Cmdr.print("Set Timeserver to: ");
    Cmdr.println(device.timeServer);
  }
  return 0;
}
bool setGMTOffset(Commander &Cmdr){
  if(DEBUGGING) debugPrintln("Setting SSID");
  /*uint8_t startOf = 0;
  int item = -1;
  //set command - find the second space char
  startOf = Cmdr.bufferString.indexOf("offset");
  startOf = Cmdr.bufferString.indexOf(' ', startOf);
  //Password will be after the space
  String ofsText = Cmdr.bufferString.substring(startOf+1);
  item = ofsText.toInt();
  if( item > -1){
    device.gmtOffset_sec = item;
  }*/
  if(Cmdr.getInt(device.gmtOffset_sec)){
    if(DEBUGGING){
      debugStartLine();
      Cmdr.print("Set GMT Offset to: ");
      Cmdr.println(device.gmtOffset_sec);
    }
    return 0;
  }
  if(DEBUGGING){
    debugStartLine();
    Cmdr.println("GMT Offset not set");
  }
  return 0;
}

bool getTime(Commander &Cmdr){
  //get network time
  if(device.wifiConnected == false){
    Cmdr.println("ESP Status: Get Time Error: Not Connected");
    return 0;
  }
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Cmdr.println("ESP Status: Failed to get time");
    return 0;
  }
  Cmdr.print("ESP Status: ");
  //HACK!!!
  Serial.println(&timeinfo, "Date %A:%B:%d:%Y Time %H:%M:%S");
  //Returns "Sunday, January 06 2019 04:55:48

  /*
    Worldwide       pool.ntp.org
    Asia            asia.pool.ntp.org
    Europe          europe.pool.ntp.org
    North America   north-america.pool.ntp.org
    Oceania         oceania.pool.ntp.org
    South America   south-america.pool.ntp.org
  */
  return 0;
}
//======================================================================================================
/*bool help(Commander &Cmdr){
  printCommandList(sBuff);
  return 0;
}*/
//======================================================================================================
bool ack(Commander &Cmdr){
  if(DEBUGGING) DEBUG.println("ACK received");
  return 0;
}
//======================================================================================================
bool nack(Commander &Cmdr){
  if(DEBUGGING) DEBUG.println("NACK received");
  return 0;
}
//======================================================================================================
bool startEnableServer(Commander &Cmdr){
  //DEBUG.println("Enabled Server");
  //Cmdr.println("ESP: Server Enabled");
  device.startServer = true;
  return 0;
}

bool disableServer(Commander &Cmdr){
  //DEBUG.println("Enabled Server");
  Cmdr.println("ESP Status: Server Disabled");
  device.startServer = false;
  disableServer();
  return 0;
}
//======================================================================================================
bool startEnablebluetooth(Commander &Cmdr){
  //DEBUG.println("Enabled BT");
  
  //Cmdr.println("ESP: BT Enabled");
  device.startBT = true;
  return 0;
}

bool disablebluetooth(Commander &Cmdr){
  //DEBUG.println("Enabled BT");
  stopBlueTooth();
  Cmdr.println("ESP Status: BT Disabled");
  device.startBT = false;
  return 0;
}
//======================================================================================================
bool startEnableUDP(Commander &Cmdr){
  //DEBUG.println("Enabled UDP");
  
  
  device.startUDP = true;
  return 0;
}//======================================================================================================
bool disableUDP(Commander &Cmdr){
  //DEBUG.println("Enabled UDP");
  stopUDP();
  Cmdr.println("ESP Status: UDP Disabled");
  device.startUDP = false;
  return 0;
}
//======================================================================================================
bool udpSendData(Commander &Cmdr){
  if(DEBUGGING) debugPrintln("Sending to UDP");
  //uint8_t startOf = 0;
  //find a space
  //startOf = Cmdr.bufferString.indexOf(':');
  if( device.udpEnabled ){
    sendUDP(Cmdr.getPayload());
    //sendUDP(Cmdr.bufferString.substring(startOf+1));
  }
  if(DEBUGGING) debugPrintln(Cmdr.getPayloadString());
  return 0;
}
//======================================================================================================
bool serverSendLine(Commander &Cmdr){
  if(DEBUGGING) debugPrintln("Sending to client");
  //uint8_t startOf = 0, endOf = 0;
  //find a space
  //startOf = Cmdr.bufferString.indexOf(':');
  //endOf = Cmdr.bufferString.length();
  if( device.serverEnabled ){
    sendToClient(Cmdr.getPayloadString()); //strip any newlines
    //sendToClient(Cmdr.bufferString.substring(startOf+1, endOf)); //strip any newlines
  }
  if(DEBUGGING) debugPrintln(Cmdr.getPayloadString());
  return 0;
}
//======================================================================================================
bool serverEndLine(Commander &Cmdr){
  if(DEBUGGING) debugPrintln("Finished sending to client");
  endClient();
  return 0;
}
//======================================================================================================
bool btSendLine(Commander &Cmdr){
  if(DEBUGGING) debugPrintln("Sending to Bluetooth");
  //uint8_t startOf = 0;
  //find a space
  //startOf = Cmdr.bufferString.indexOf(':');
  if( device.btEnabled ){
    SerialBlueTooth.println(Cmdr.getPayloadString());
    //SerialBlueTooth.println(Cmdr.bufferString.substring(startOf+1));
  }
  if(DEBUGGING) debugPrintln(Cmdr.getPayloadString());
  return 0;
}

bool setUDPPort(Commander &Cmdr){
  if(DEBUGGING) debugPrintln("Setting UDP Port");
  /*uint8_t startOf = 0;
  int port = -1;
  //set command - 'set UDPPort 1234' find the second space char
  startOf = Cmdr.bufferString.indexOf("UDPPort");
  startOf = Cmdr.bufferString.indexOf(' ', startOf);
  String prt = Cmdr.bufferString.substring(startOf+1);
  port = prt.toInt();
  if( port > 0 ){
    device.udpPort = port;
  }*/
  if(Cmdr.getInt(device.udpPort)){
    if(device.udpPort < 1) device.udpPort = 1;
  }
  if(DEBUGGING){
    debugStartLine();
    Serial.print("Set UDP Port to: ");
    Serial.println(device.udpPort);
  }
  return 0;
}
bool setUDPAddress(Commander &Cmdr){
  if(DEBUGGING) debugPrintln("Setting UDP Address");
  /*uint8_t startOf = 0;
  uint8_t endOf = 0;
  //set command - 'set UDPAddress 123.456.789.0' find the second space char
  startOf = Cmdr.bufferString.indexOf("UDPAddress");
  startOf = Cmdr.bufferString.indexOf(' ', startOf);
  endOf = Cmdr.bufferString.length();
  //device.UDPAddress = Cmdr.bufferString.substring(startOf+1, endOf);*/
  device.UDPAddress = Cmdr.getPayloadString();
  if(DEBUGGING){
    debugStartLine();
    Serial.print("Set UDP Address to: ");
    Serial.println(device.UDPAddress);
  }
  return 0;
}
//======================================================================================================
//Standard command handlers
//======================================================================================================
/*bool unknownCommand(Commander &Cmdr){
  //ERRORS.println("Unknown Serial Command");
  //Cmdr.println("nack");
  return 0;
}
//======================================================================================================
bool printCommandList(Commander &Cmdr){
  //Prints all the commands
  uint8_t n = 0;
   Cmdr.println("ESP Status: Command List:");
  for(n = 0; n < numOfCmds; n++){
    Cmdr.println(commands[n].commandString);
  }
  Cmdr.println("ESP Status: Command List End");
  return 0;
}*/
//======================================================================================================

//SERIAL PORT PROCESSING --------------------------------------------------
/*
void checkSerial(){
  while(Serial.available()){
    //DEBUG.println("buffering");
    bufferSerial(Serial.read(), sBuffer);
  }
  if(sBuffer.newLine){
    
    //DEBUG.println("Finding command");
    //sBuffer.commandVal = matchCommand(sBuffer);
    sBuffer.commandVal = matchCommand(sBuffer.bufferString);
    //DEBUG.println("Handling command");
    handleCommand(sBuffer);
    //if(!commands[commandIndex].handler(sBuffer)) Serial.println("Command Handled OK");
    //resetBuffer(sBuffer); //no need - resets in handler func
  }
}

void bufferSerial(uint8_t dataByte, serialBuffer_t &sBuff){
  switch(Cmdr.parseState){
    case WAITING_FOR_START:
      //DEBUG.println("Waiting for Start");
      if( isCommandStart(dataByte) ) {
        Cmdr.parseState = BUFFERING_PACKET;
        writeToBuffer(dataByte, sBuff);
        if(Cmdr.bufferFull) resetBuffer(sBuff);
      }
      break;
    case BUFFERING_PACKET:
      //DEBUG.println("Buffering");
      writeToBuffer(dataByte, sBuff);
      if(Cmdr.bufferFull) resetBuffer(sBuff);//dump the buffer
      if(Cmdr.newLine == true) {
        Cmdr.bufferString.trim(); //remove any whitespace
        //Add back the newline
        //Cmdr.bufferString += Cmdr.END_OF_PACKET;
        Cmdr.parseState = PACKET_RECEIVED;
        //DEBUG.println("Newline Detected");
      } //unpack the data
      break;
    case PACKET_RECEIVED:
      //DEBUG.println("Packet received ...?");
      //if you get here then the packet has not been unpacked and more data is coming in. It may be a stray newline ... do nothing
      break;  
  }
}

void writeToBuffer(uint8_t dataByte, serialBuffer_t &sBuff){
  if(Cmdr.bytesWritten == SBUFFER-1){
    Cmdr.bufferFull = true; //buffer is full
    return;
  }
  //Cmdr.buf[Cmdr.bytesWritten] = dataByte;
  Cmdr.bufferString += (char)dataByte;
  if(dataByte == Cmdr.END_OF_PACKET) Cmdr.newLine = true;
  Cmdr.bytesWritten++;
}
void resetBuffer(serialBuffer_t &sBuff){
  //DEBUG.println("Resetting Buffer");
  //Cmdr.readIndex = 0;
  //Cmdr.bytesWritten = 0;
  Cmdr.bytesWritten = 0;
  Cmdr.newData = false;
  Cmdr.newLine = false;
  Cmdr.bufferFull = false;
  Cmdr.parseState = WAITING_FOR_START;
  //Cmdr.endOfFile = false;
  Cmdr.bufferString = "";
}

uint8_t matchCommand(String cmd){
  //using Strings instead of char arrays
  //loop through the command list and see if it appears in the sBuff String
  for(uint8_t n = 0; n < numOfCmds; n++){
    //if( cmd.indexOf( commands[n].commandString ) > -1) return n;
    if( cmd.startsWith( commands[n].commandString ) ) return n;
  }
  return 255;
}

uint8_t matchCommand(serialBuffer_t &sBuff){
  //using Strings instead of char arrays
  //loop through the command list and see if it appears in the sBuff String
  for(uint8_t n = 0; n < numOfCmds; n++){
    //Serial.print("Checking command: ");
    //Serial.println(commands[n].commandString);
    //Serial.print("Serial Line is: ");
    //Serial.println( Cmdr.bufferString );
    if( Cmdr.bufferString.indexOf( commands[n].commandString ) > -1) return n;
  }
  return 255;
}


bool handleCommand(serialBuffer_t &sBuff){
  bool returnVal = false;
  if(Cmdr.commandVal == 255) {
    //DEBUG.println("handleCommand: Commandval is 255 : Call unknown cmd handler");
    returnVal = unknownCommand(sBuff);
  } //unknown command
  //Call the function in the function pointer array that matches the command
  //Pass the port object so the command handler knows which port to reply to
  else {
    //DEBUG.print("handleCommand: Calling command handler. Index: "); 
    //DEBUG.println(Cmdr.commandVal);
    returnVal =  commands[Cmdr.commandVal].handler(sBuff);
    //returnVal =  (*commandHandlers[Cmdr.commandVal])(sBuff);
  }
  resetBuffer(sBuff);
  return returnVal;
}

bool isCommandStart(char dataByte){
  //The start of the command must be a typeable character
  if( dataByte > 31 &&  dataByte < 127) return true;
  return false;
}*/
