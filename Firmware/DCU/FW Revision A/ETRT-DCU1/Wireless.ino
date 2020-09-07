////http client strings sent to the ESP need to be prefixed with serverSend: and end with a newline so the ESP knows what to do with them
const String header = "serverSend: <!DOCTYPE html><html><body>\n";
const String configPageTitle = "serverSend: <p>Enactive Torch RT Configuration page.</p>";
const String pageEnd = "serverSend: </body></html>";

//const String serverCommandString1 = "<br><br><center><p>Command X received.</p></center><br>"
//Command forms
const String serverCommandString1 = "serverSend: <br><br><center><form>Command: <input type=\"text\" name=\"Command\" value=\"\" size=\"64\"> <br><br><input type=\"submit\" value=\"Submit\"></form></center>";
const String serverCommandString2 = "serverSend: <center><p>Click the \"Submit\" button to send the command to the Enactive Torch.</p></center>";

const String replyMsgStart = "serverSend: <br><br><center><p>";
const String replyMsgEnd = "</p></center><br>";

const String paraStart = "<center><p>";
const String paraEnd = "</p></center>";

void processServerCommandRequest(Commander &Cmdr){
  String cmdStr = "";
  cmdStr = Cmdr.bufferString.substring(Cmdr.bufferString.indexOf('=')+1, Cmdr.bufferString.indexOf("HTTP")-2);
  cmdStr += '\n'; //add a newline just in case its needed
  //Add to the buffer as a pending command
  //Some pending commands may require a reply to the server ...?
  if( cmdStr.indexOf("help") > -1 ){
    //command requested a list of commands so use the stream option
    serverSendCommandList(Cmdr);
  }else{
    Cmdr.loadString(cmdStr);
    //Cmdr.isCommandPending = true;
    String replyMessage = "Received Command: ";
    replyMessage += cmdStr;
    serverSendCommandPage(Cmdr, replyMessage);
  }
  //If a command is part of the request then extract the command and place in pendingCommandString,
  //Set the isCommandPending flag to true
  //The main process will then handle the command
}


void serverSendCommandPage(Commander &Cmdr, String replyMessage){
  //send the command page with the attached message
  //String replymsg = replyMsgStart;
  //replymsg += replyMessage;
  //replymsg += replyMsgEnd;
  Cmdr.println(header);
  Cmdr.println(serverCommandString1);
  Cmdr.println(serverCommandString2);
  Cmdr.println(replyMsgStart + replyMessage + replyMsgEnd);
  Cmdr.println(pageEnd);
}


void serverSendCommandList(Commander &Cmdr){
  //send the list of commands as HTML
  Cmdr.println(header);
  Cmdr.println(serverCommandString1);
  Cmdr.println(serverCommandString2);
  //Prints all the commands
  uint8_t n = 0;
  Cmdr.println("serverSend: <center><p>COMMAND LIST</p></center>");
  //String cmdList = "serverSend:";

  Cmdr.setPrefix("serverSend: <p>");
  Cmdr.setPostfix("</p>");
  Cmdr.printCommandList();
  /*for(n = 0; n < numOfCmds; n++){
    cmdList += paraStart;
    //cmdList += commands[n].commandString;
    cmdList += paraEnd;
  }*/
  //Cmdr.println(cmdList);
  Cmdr.println(pageEnd);
}

String sendServerMessage1(){
  //just for testing - return a line to the server
  WIRELESS.print("serverSend ");
  WIRELESS.println(header);
  WIRELESS.print("serverSend ");
  WIRELESS.println(header);
  WIRELESS.print("serverSend ");
  WIRELESS.println(header);
  WIRELESS.print("serverEnd");
}




void wirelessSerialLoopbackTest(){
  
  DEBUG.println("Testing WiFi Serial");
  WIRELESS.println("Testing- Sending this");
  WIRELESS.flush();
  while(WIRELESS.available()){
    DEBUG.write(WIRELESS.read());
  }
  DEBUG.println("Wifi serial test OK");
}




void startESPBootloader(){
  WIRELESS.end();
  pinMode(PB12, INPUT);
  pinMode(PB13, INPUT);
  disableESP();
  //disablePeripheralPower(); //Switch on the voltage regulator
  delay(100);
  enableESPBootloader();
  delay(100);
  enableESP();
}

void startESPApplication(){
  disableESP();
  
  WIRELESS.begin(115200);
  //disablePeripheralPower(); //Switch on the voltage regulator
  delay(100);
  disableESPBootloader();
  delay(100);
  enableESP();
  device.espEnabled = false;
  device.espBootMode = false;
  device.espServerEnabled = false;
  device.espWiFiConnected = false;
  device.espWiFiConnectionErrors = 0;
  device.espUDPEnabled = false;
  device.espBTEnabled = false;
}
