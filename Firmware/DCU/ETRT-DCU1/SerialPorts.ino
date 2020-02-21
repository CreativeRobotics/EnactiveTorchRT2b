//fill two buffers for the serial ports, one for USB and one for WiFi


/*
void setPortMode(serialBuffer_t &sBuff, serialPortMode_t pMode){
  sBuff.mode = pMode;
}*/

/*
bool checkUSB(){
  return cmdDCU.update();
  //check the USB serial buffer
  //return checkPort(bufferUSB);
}

bool checkWireless(){
  return cmdWireless.update();
  //check the wireless serial buffer
  //return checkPort(bufferWireless);
}*/
/*
bool checkPort(serialBuffer_t &sBuff){
  //Generic serial port handler
  //DEBUG.println("Checking Port");
  bool retStatus = false;
  switch(sBuff.mode){
    case NO_OPERATION: //no operation
        //DEBUG.println("NO OP MODE");
      while(sBuff.port->available()){
        sBuff.port->read();
      }
      break;
    case COMMAND_MODE: //command mode
        //DEBUG.println("CMD MODE");
      while(sBuff.port->available()){
        processBuffer(sBuff, sBuff.port->read());
      }
      if(sBuff.newLine == true) return handlePacket(sBuff); //returns true if data was unpacked
      break;
    case PASSTHROUGH: //pass through
        //DEBUG.println("PASS MODE");
      passThrough(sBuff);
      break;
    case PASSTHROUGH_COMMAND: //command mode with raw bytes copied to other port
        //DEBUG.println("PASS CMD MODE");
      while(sBuff.port->available()){
        sBuff.altPort->write( sBuff.port->peek() );
        processBuffer(sBuff, sBuff.port->read());
      }
      if(sBuff.newLine == true) return handlePacket(sBuff); //returns true if data was unpacked
      break;
    case FILEREADER_TOSERIAL: //read the file and pass straight to the serial port
      while(sBuff.fileReader.available()){
        sBuff.port->write(sBuff.fileReader.read());
      }
      sBuff.endOfFile = true; //reach here and the file has been read
      break;
    case FILEREADER_COMMAND: //read the file which should be open and pass that to the command handlers
      while(sBuff.fileReader.available()){
        //DEBUG.print("Reading file. Available = "); DEBUG.println(sBuff.fileReader.available());
        char fileByte = sBuff.fileReader.read();
        //DEBUG.print("Sending to buffer: "); DEBUG.println(fileByte);
        processBuffer(sBuff, fileByte);
        if(sBuff.newLine == true){
          //Return true if there are more lines to read
          //Skip if the line begins with #
          sBuff.bufferString.trim(); //remove leading or trailing whitespace
          if(sBuff.bufferString.startsWith("#")){
            resetBuffer(sBuff);
            return !sBuff.endOfFile;
          }
          handlePacket(sBuff); //returns true if data was unpacked
          return !sBuff.endOfFile;
        }
      }
      sBuff.endOfFile = true; //reach here and the file has been read
      return !sBuff.endOfFile;
      break;
  }
  return false;
}

void passThrough(serialBuffer_t &sBuff){
  while(sBuff.port->available()) sBuff.altPort->write( sBuff.port->read() );
}

bool handlePacket(serialBuffer_t &sBuff){
  //extract the commands and call the correct command handler
  //This should handle lines with multiple commands?
  //Command Val is generated from command string and used to pick the function
  //DEBUG.println("Handling Packet");
  sBuff.commandVal = matchCommand(sBuff.bufferString);
  //Handle the command
  return handleCommand(sBuff);
}

bool handleCommand(serialBuffer_t &sBuff){
  bool returnVal = false;
  if(sBuff.commandVal == 255) {
    //DEBUG.println("handleCommand: Commandval is 255 : Call unknown cmd handler");
    returnVal = unknownCommand(sBuff);
  } //unknown command
  //Call the function in the function pointer array that matches the command
  //Pass the port object so the command handler now which port to reply to
  else {
    //DEBUG.print("handleCommand: Calling command handler. Index: "); 
    //DEBUG.println(sBuff.commandVal);
    returnVal =  commands[sBuff.commandVal].handler(sBuff);
    //returnVal =  (*commandHandlers[sBuff.commandVal])(sBuff);
  }
  resetBuffer(sBuff);
  return returnVal;
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
void processBuffer(serialBuffer_t &sBuff, uint8_t dataByte){
  //uint8_t dataByte;
  //read one byte and process
  //dataByte = dataRead;//sBuff.port->read();
  //DEBUG.print("Buffer ");
  //DEBUG.println(dataByte);
  if(dataByte == -1) return; //no actual data to process
  switch(sBuff.parseState){
    case WAITING_FOR_START:
      //DEBUG.println("Waiting for Start");
      if( isCommandStart(dataByte) ) {
        sBuff.parseState = BUFFERING_PACKET;
        writeToBuffer(dataByte, sBuff);
        if(sBuff.bufferFull) resetBuffer(sBuff);
      }
      break;
    case BUFFERING_PACKET:
      //DEBUG.println("Buffering");
      writeToBuffer(dataByte, sBuff);
      if(sBuff.bufferFull) resetBuffer(sBuff);//dump the buffer
      if(sBuff.newLine == true) {
        sBuff.parseState = PACKET_RECEIVED;
        //DEBUG.println("Newline Detected");
      } //unpack the data
      break;
    case PACKET_RECEIVED:
      //DEBUG.println("Packer received ...?");
      //if you get here then the packet has not been unpacked and more data is coming in. It may be a stray newline ... do nothing
      break;  
  }
}

void writeToBuffer(uint8_t dataByte, serialBuffer_t &sBuff){
  if(sBuff.bytesWritten == SBUFFER-1){
    sBuff.bufferFull = true; //buffer is full
    return;
  }
  //sBuff.buf[sBuff.bytesWritten] = dataByte;
  sBuff.bufferString += (char)dataByte;
  if(dataByte == sBuff.END_OF_PACKET) sBuff.newLine = true;
  sBuff.bytesWritten++;
}


void resetBuffer(serialBuffer_t &sBuff){
  //DEBUG.println("Resetting Buffer");
  //sBuff.readIndex = 0;
  //sBuff.bytesWritten = 0;
  //Check the pendingCommand flag, if true then copy the pending command string to the buffer so it will be handled next time around
  
  sBuff.bytesWritten = 0;
  sBuff.newData = false;
  sBuff.newLine = false;
  sBuff.bufferFull = false;
  sBuff.parseState = WAITING_FOR_START;
  if(sBuff.isCommandPending){
    sBuff.bufferString = sBuff.pendingCommandString;
    sBuff.isCommandPending = false;
    sBuff.pendingCommandString = "";
    
  }else sBuff.bufferString = "";
  //sBuff.endOfFile = false;
}

bool isCommandChar(char dataByte){
  //Command chars are letters and a few other characters
  //DEBUG.print("Databyte is ");
  //DEBUG.println(dataByte);
  if(dataByte < 63 || dataByte > 126) return false;
  return true;
}

bool isCommandStart(char dataByte){
  //The start of the command must be a typeable character
  if( dataByte > 31 &&  dataByte < 127) return true;
  return false;
}

bool isEndOfLine(char dataByte){
  if(dataByte == 13 || dataByte == 10) return true;
  return false;
}*/
