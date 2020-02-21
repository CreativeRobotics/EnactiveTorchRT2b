//class for handling communication between the computer and a single interface unit

//A global serial event handler
void serialEvent(Serial port) {
  //if(UnitA.printDebugMessages || UnitB.printDebugMessages || UnitC.printDebugMessages || UnitD.printDebugMessages) println("Serial Event:" + port);
  //if(port == UnitA.serialPort) UnitA.serialEventHandler();
  //if(port == UnitB.serialPort) UnitB.serialEventHandler();
  //if(port == UnitC.serialPort) UnitC.serialEventHandler();
  //if(port == UnitD.serialPort) UnitD.serialEventHandler();
  for(int n = 0; n < numberOfUnits; n++){
    if(User[n].printDebugMessages == true) println("Serial Event:" + port);
    //println("Serial Event:" + port);
    //println("Serial Event: " + port);
    if(port == User[n].serialPort) User[n].serialEventHandler();
  }
}


boolean NOUnitsConnected(){
  //return true if there are no connected units
  for(int n = 0; n < numberOfUnits; n++){
    if(User[n].portConnected == true) return false;
  }
  return true;
}
void LEDsOn(){
  for(int n = 0; n < numberOfUnits; n++){
    User[n].sendLEDOn();
  }
}
void LEDsOff(){
  for(int n = 0; n < numberOfUnits; n++){
    User[n].sendLEDOff();
  }
}
void beepsOn(){
  for(int n = 0; n < numberOfUnits; n++){
    User[n].sendBeepOn();
  }
}
void beepsOff(){
  for(int n = 0; n < numberOfUnits; n++){
    User[n].sendBeepOff();
  }
}
void ledBeepOn(){
  for(int n = 0; n < numberOfUnits; n++){
    User[n].sendLEDOn();
    User[n].sendBeepOn();
  }
}
void ledBeepOff(){
  for(int n = 0; n < numberOfUnits; n++){
    User[n].sendLEDOff();
    User[n].sendBeepOff();
  }
}

void setHaptics(){
  //set all haptics
  //use trial mappings to get user haptics and send to correct unit
  //Trial.getUsersGame(0);
  //int userNo = 0;
  for(int n = 0; n < numberOfUnits; n++){
    //this assumes that the environment and users were updated by passing user objects to the appropriate game
    User[n].updateHaptics();
  }
}

void sendStartRound(String startMessage){
  //Send to all units at once
  for(int n = 0; n < numberOfUnits; n++){
    User[n].sendStartRound(startMessage);
  }
}


void sendEndRoundMessage(String msg){
  for(int n = 0; n < numberOfUnits; n++){
    User[n].setHapticsMessage(0, 0, msg);
  }
}
//=====================================================================================================================================================================

void sendHaltRoundAll(boolean aborted){
  //halt the trial now
  for(int n = 0; n < numberOfUnits; n++){
    if(aborted)  User[n].sendEndRound(trialAbortedMessage);
    else         User[n].sendEndRound(endTrialMessage);
  }
  Trial.roundActive = false;
  Trial.startIndicatorOn = false;
  Trial.middleIndicatorOn = false;
  Trial.middleIndicatorDone = false;
}


//data comms



static char LINEFEED = 10;
static char NEWLINE = 10;
static char CARRAIGERETURN = 13;



class PCInterfaceUnit 
{
  public
  Serial serialPort;
  String portName;
  int portIndex = -1;
  int serialLinesReceived  =0;
  boolean surpress = false;
  boolean waitingForReply = false;
  boolean newData = false;
  int errorCount = 0;
  int timeouts = 0;
  long responseTime = 0;
  long sentTime = 0;
  boolean portConnected = false;
  int unitNumber;
  boolean printDebugMessages = false;
  boolean printSerialLines = false;
  int trackerballMode = 0; //0 = use X, 1 = use Y, 2 = use(X-Y)/2, 3 = use (X-Y)/2
  int xChange = 0;
  int yChange = 0;
  //int userPos = 0;
  int elapsedTime = 0;
  int LEDBrightness = 0;
  int haptic0Value = 0;
  int haptic0Default = 1023;
  int haptic1Value = 0;
  int haptic1Default = 1023;
  boolean mirrorHaptics = true; //when true haptic1 is the same as haptic0
  int buttonState = 0;
  
  //Complete commands
  String LED_ON = 'L' + String.valueOf(NEWLINE); 
  String LED_OFF = 'l' + String.valueOf(NEWLINE); 
  String BEEP_ON = 'B' + String.valueOf(NEWLINE); 
  String BEEP_OFF = 'b' + String.valueOf(NEWLINE); 
  String QUERY = "?" + String.valueOf(NEWLINE);
  String GET_STATE = 'r' + String.valueOf(NEWLINE);
  
  //Command characters for the start of a message
  String HAPTICS_CMD = "h ";
  String HAPTICS_MESSAGE_CMD = "H ";
  String START_ROUND_CMD = "s ";
  String END_ROUND_CMD = "e ";
  String GET_STATE_MESSAGE_CMD = "R ";
  String GET_STATE_CMD = "r";
  
  
  //Serial Commands
  char REQUEST_DATA             =  'r'; //no pl
  char REQUEST_DATA_SET_MESSAGE =  'R'; ////pl
  char START_TRIAL              =  's'; //pl
  char END_TRIAL                =  'e';
  char SET_HAPTICS              =  'h';
  char SET_HAPTICS_SET_MESSAGE  =  'H';
  //Constructor
  PCInterfaceUnit(int uNo) {
    unitNumber = uNo;
  }
  //-----------------------------------------------------------------------
  void connectPort(PApplet pa){
     // get the first available port (use EITHER this OR the specific port code below)
    if(portConnected || portIndex < 0) return;
    String tmpPortName = portList[portIndex];
    portName = tmpPortName;
    println(portName);
    // open the serial port
    serialPort = new Serial(pa, portName, 115200); 
    portConnected = true;
    int nl = 10;      // ASCII linefeed
    serialPort.bufferUntil(nl);
    println("Connected Unit " + String.valueOf(unitNumber) + " To " + portName);
  }
  //---------------------------------------------------------------------------
  void disconnectPort(){
    serialPort.stop();
    serialPort = null;
    portConnected = false;
    println("Disconnected "+String.valueOf(unitNumber));
  }
  //---------------------------------------------------------------------------
  void checkPort(){
    //check the serial port for new data 
    if(serialPort == null || portConnected == false) return;
    
    //Collect any data
  }
  //---------------------------------------------------------------------------
  void serialEventHandler(){
    if(serialPort == null) return; 
    try {
      if(printDebugMessages) println("Trying Serial Handler " + String.valueOf(unitNumber));
      //println("Trying Serial Handler " + String.valueOf(unitNumber));
      String serialLine = serialPort.readString();
      if(printSerialLines) println(serialLine);
      if (serialLine != null){
        //if(debugPrint) println("String not NULL");
        serialLinesReceived++;
        //parse the data
        parseData( serialLine );
        //cp5.get(Textlabel.class,"SerialTextA").setText(serialLine);
        //logDataA(serialLine); //log the data if requested
      }
        //else if(debugPrint) println("Null serial string A");
    }
    catch(RuntimeException e) {
      println("SerialA Error " + e);
      errorCount++;
    } 
  }
  //---------------------------------------------------------------------------
  void parseData(String parseThis){
    if(printDebugMessages) println("Parsing . . . " + parseThis);
    if(parseThis.length() < 2) return;
    String delims = "[ ,=\r\n]+"; 
    String[] tokens = parseThis.split(delims);
    //tokens is now an array of items.
    if(tokens[0] == "OK"){//ACK
      if(printDebugMessages) print("Recieved ACK A");
      waitingForReply = false;
    }
    else if(tokens[0] == "ERR"){//NACK
      if(printDebugMessages) print("Recieved ERR A");
      errorCount++;
      waitingForReply = false;
    }
    else if(tokens.length == 5){
      
      if(printDebugMessages) print("Unpacking ");
      //There is only one data packet type, so parse it into appropriate variables
      elapsedTime = Integer.parseInt(tokens[0]);
      if(printDebugMessages) {print(elapsedTime);print("  ");}
      
      xChange = Integer.parseInt(tokens[1]);
      if(printDebugMessages) {print(xChange);print("  ");}
      
      yChange = Integer.parseInt(tokens[2]);
      if(printDebugMessages) {print(yChange);print("  ");}
      
      buttonState = Integer.parseInt(tokens[3]);
      if(printDebugMessages) {print(buttonState);print("  ");}
  
      LEDBrightness = Integer.parseInt(tokens[4]);
      if(printDebugMessages) { print(LEDBrightness);print("  ");}
      
      setReplyState_OK();
    }
    else if(printDebugMessages) print("Parser Error " + tokens);
    if(printDebugMessages) println(" Done");
  }

  void resetVariables(){
    newData = false;
    waitingForReply = false;
    xChange = 0;
    yChange = 0;
    elapsedTime = 0;
    LEDBrightness = 0;
    haptic0Value = 0;
    haptic1Value = 0;
    buttonState = 0;
  }
  //---------------------------------------------------------------------------
  void setReplyState_OK(){ 
      newData = true;
      waitingForReply = false;
  } 
  //---------------------------------------------------------------------------
  void resetTimeout(){ 
      newData = false;
      waitingForReply = false;
      timeouts++;
  }
  //---------------------------------------------------------------------------
  void sendData(String data){
    if(portConnected == false || serialPort == null) return;
    serialPort.write(data);
    //println("Sent unit "+ String.valueOf(unitNumber) + " Data: " + data);
    if(printDebugMessages) println("Sent "+String.valueOf(unitNumber) +data);
  }
  //---------------------------------------------------------------------------
  void sendStartRound(String lcdMessage){
    String message = START_ROUND_CMD + lcdMessage + String.valueOf(NEWLINE);
    sendData(message);
    waitingForReply = true;
  }
  //---------------------------------------------------------------------------
  void sendEndRound(String lcdMessage){
    String message = END_ROUND_CMD + lcdMessage + String.valueOf(NEWLINE);
    sendData(message);
    waitingForReply = true;
  }
  //---------------------------------------------------------------------------
  void updateHaptics(){
    String message = HAPTICS_CMD + String.valueOf(haptic0Value) + ' ' + String.valueOf(haptic1Value) + String.valueOf(NEWLINE);
    sendData(message);
  }
  
  void updateHapticsMessage(String lcdMessage){
    String message = HAPTICS_CMD + String.valueOf(haptic0Value) + ' ' + String.valueOf(haptic1Value) + ' ' + lcdMessage + String.valueOf(NEWLINE);
    sendData(message);
  }
  
  void setHaptics(int h0, int h1){
    haptic0Value = h0;
    haptic1Value = h1;
    String message = HAPTICS_CMD + String.valueOf(haptic0Value) + ' ' + String.valueOf(haptic1Value) + String.valueOf(NEWLINE);
    sendData(message);
  }
  //---------------------------------------------------------------------------
  void setHapticsMessage(int h0, int h1, String lcdMessage){
    haptic0Value = h0;
    haptic1Value = h1;
    String message = HAPTICS_MESSAGE_CMD + String.valueOf(h0) + ' ' + String.valueOf(h1) + ' ' + lcdMessage + String.valueOf(NEWLINE);
    sendData(message);
  }
  //---------------------------------------------------------------------------
  void getState(){
    sendData(GET_STATE);
    waitingForReply = true;
  }
  //---------------------------------------------------------------------------
  void getStateMessage(String lcdMessage){
    String message = GET_STATE_MESSAGE_CMD + lcdMessage + String.valueOf(NEWLINE);
    sendData(message);
    waitingForReply = true;
  }
  //---------------------------------------------------------------------------
  void sendBeepOn(){ sendData(BEEP_ON);}
  //---------------------------------------------------------------------------
  void sendBeepOff(){sendData(BEEP_OFF);}
  //---------------------------------------------------------------------------
  void sendLEDOn(){sendData(LED_ON);}
  //---------------------------------------------------------------------------
  void sendLEDOff(){sendData(LED_OFF);}
  //---------------------------------------------------------------------------
  void queryDevice(){sendData(QUERY);}
  
  int getUserChange(){
    switch(trackerballMode){
      case 0: 
        return xChange;
      case 1: 
        return yChange;
      case 2: 
        return (xChange+yChange)/2;
      case 3: 
        return (xChange-yChange)/2;
    }
    return 0;
  }
  void resetUserChange(){
    xChange = 0;
    yChange = 0;
  }
}
