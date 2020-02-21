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
  {"r",        getDataHandler,    "Request data"},
  {"R",        getDataMessageHandler,    "Request data set message"},
  {"s",        startTrialHandler,    "Start Trial"},
  {"e",        stopTrialHandler,    "Stop Trial"},
  {"h",        setHapticsHandler,    "Set Haptics"},
  {"H",        setHapticsMessageHandler,    "Set Haptics and Message"},
  {"?",        queryHandler,    "Query"},
  {"L",        ledOnHandler,    "LED On"},
  {"l",        ledOffHandler,    "LED Off"},
  {"B",        beepOnHandler,    "Beep On"},
  {"b",        beepOffHandler,    "Beep Off"},
  {"stream",   streamHandler,    "toggle stream"},
  {"swap xy",   swapXYHandler,    "Swap trackerball X and Y"},
  {"invert x",   invertXHandler,    "Invert X step"},
  {"invert y",   invertYHandler,    "invert Y step"},
  {"ptoggle",   toggle5VHandler,    "toggle 5V power on and off"},
  {"message",   sendMessageHandler,    "Send a message to the screen"},
  {"test tft",   testTftHandler,    "test the screen"},
  {"set cursor",   setCursorHandler,    "set the position for text"},
  {"set size",   setTextSizeHandler,    "set the size of text"},
  {"data display",   displayDataHandler,    "togle data on screen"},
};

const uint16_t numOfDCUCmds = sizeof(DCUCommands);

//const uint16_t numOfCmds = sizeof(commands) /  sizeof(commands[0]); //calculate the number of commands so we know the array bounds
/*
bool commandFunctionTemplate( Commander &Cmdr ){

  return 0;
}
*/
/*
int getCommandEndIndex(Commander &Cmdr){
  //find the end of the characters in the command string - index AFTER the last character in the command string
  return commands[Cmdr.commandVal].commandString.length();
}*/


bool getTrueOrFalse(String dataString){
  if(dataString.indexOf("true") > -1) return true;
  return false;
}
//--------------------------------------------------------------------------------------------------------
bool getDataHandler( Commander &Cmdr){
  printData();
  if(drawTheAgent)drawAgent();
  packetsReceived++;
  return false;
}
bool getDataMessageHandler( Commander &Cmdr){
  displayMessage = Cmdr.getPayloadString();
  printTimerMessage(displayMessage);
  if(drawTheAgent)drawAgent();
  printData();
  packetsReceived++;
  return false;
}
bool startTrialHandler( Commander &Cmdr){
  displayMessage = Cmdr.getPayloadString();
  clearPrintMessage(displayMessage, tftXCursor, tftYCursor, tftTextSize);
  startTrial();
  sendACK();
  packetsReceived++;
  return false;
}
bool stopTrialHandler( Commander &Cmdr){
  displayMessage = Cmdr.getPayloadString();
  clearPrintMessage(displayMessage, tftXCursor, tftYCursor, tftTextSize);
  stopTrial();
  sendACK();
  packetsReceived++;
  return false;
}
bool setHapticsHandler( Commander &Cmdr){
  Cmdr.getInt(hapticRate1);
  Cmdr.getInt(hapticRate2);
  packetsReceived++;
  return false;
}
bool setHapticsMessageHandler( Commander &Cmdr){
  Cmdr.getInt(hapticRate1);
  Cmdr.getInt(hapticRate2);
  packetsReceived++;
  
  setHaptics();
  displayMessage = Cmdr.getPayloadString();
  clearPrintMessage(displayMessage, tftXCursor, tftYCursor, tftTextSize);
  sendACK();
  packetsReceived++;
  return false;
}
bool ledOnHandler( Commander &Cmdr){
  userLED_ON();
  manualFlash = 1;
  packetsReceived++;
  return false;
}
bool ledOffHandler( Commander &Cmdr){
  userLED_OFF();
  manualFlash = 0;
  packetsReceived++;
  return false;
}
bool beepOnHandler( Commander &Cmdr){
  beepON();
  packetsReceived++;
  return false;
}
bool beepOffHandler( Commander &Cmdr){
  beepOFF();
  packetsReceived++;
  return false;
}
bool queryHandler( Commander &Cmdr){
  sendACK();
  packetsReceived++;
  return false;
}
bool streamHandler( Commander &Cmdr){
  streamData = !streamData;
  packetsReceived++;
  return false;
}
bool swapXYHandler( Commander &Cmdr){
  stopTrackerball();
  swapXY = !swapXY;
  startTrackerball(swapXY, trackXDirection, trackYDirection);
  packetsReceived++;
  return false;
}

bool invertXHandler( Commander &Cmdr){
  trackXDirection = -trackXDirection;
  packetsReceived++;
  return false;
}
bool invertYHandler( Commander &Cmdr){
  trackYDirection = -trackYDirection;
  packetsReceived++;
  return false;
}
bool toggle5VHandler( Commander &Cmdr){
  if(v5PowerOn) disable5VPower();
  else          enable5VPower();
  packetsReceived++;
  return false;
}

bool sendMessageHandler( Commander &Cmdr){
  clearPrintMessage(Cmdr.getPayload(), tftXCursor, tftYCursor, tftTextSize);
  Cmdr.print("Message: ");
  Cmdr.print(Cmdr.getPayload());
  packetsReceived++;
  return false;
}

bool testTftHandler( Commander &Cmdr){
  tftTest();
  packetsReceived++;
  return false;
}

bool setCursorHandler( Commander &Cmdr){
  Cmdr.getInt(tftXCursor);
  Cmdr.getInt(tftYCursor);
  packetsReceived++;
  return false;
}

bool setTextSizeHandler( Commander &Cmdr){
  Cmdr.getInt(tftTextSize);
  packetsReceived++;
  return false;
}

bool displayDataHandler( Commander &Cmdr){
  displayData = !displayData;
  displayData ? Cmdr.println("Data display ON") : Cmdr.println("Data display OFF");
  if(displayData) startDisplayData();
  packetsReceived++;
  return false;
}


void sendACK(){
  USB.println("OK");
  USB.flush();
}
void sendNACK(){
  USB.println("ERR");
  USB.flush();
}
