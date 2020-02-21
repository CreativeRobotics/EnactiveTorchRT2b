/*void waitForStart(){
  //wait here until the ETRT sends a configuration command
  WiFi.begin(myssid, password);
  //connectToWiFi()
  server.begin();

  SerialBT.begin("ETRT_V2");
}*/


//void setPortMode(serialBuffer_t &sBuff, serialPortMode_t pMode){
  //Cmdr.mode = pMode;
//}

void initSerialPort(){
  Serial.begin(115200);
  cmdESP.setBuffer(256);
  cmdESP.begin(&Serial, ESPCommands, numOfESPCmds);
  cmdESP.commanderName = "ETRT-ESP32>";
  cmdESP.printCommandPrompt();
  cmdESP.println(numOfESPCmds);
  cmdESP.printCommandPrompt();
  /*resetBuffer(sBuffer);
  sBuffer.bufferString.reserve(SBUFFER);
  sBuffer.port = &Serial;
  //Setup pass through ports
  sBuffer.altPort = &Serial;
  setPortMode(sBuffer, COMMAND_MODE); */
  //DEBUG.println("Serial initialised");
}
