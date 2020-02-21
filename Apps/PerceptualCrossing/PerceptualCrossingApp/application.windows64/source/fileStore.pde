int boolToInt(boolean b) {
    return Boolean.compare(b, false);
}
void writeFileHeader(){
  if(logCombinedOpen == true){
    //First write a few parameters
    
    
    LogFileCombined.print(" Trial "+Trial.trialNumber);
    LogFileCombined.println(" Round "+Trial.roundNumber);
    
    LogFileCombined.print("World width = "+environmentWidth);
    LogFileCombined.print(" Trial Length = "+roundLengthSeconds);
    LogFileCombined.println(" Trial steps per second = "+roundResolution);
    
    
    
    for(int n = 0; n < numberOfUnits; n++){
      int usersGame;
      int usersSlot;
      usersGame = Trial.getUsersGame(n);
      usersSlot = Trial.getPlayerSlot(n);
      LogFileCombined.print("User " + n);
      LogFileCombined.print(" (Subject ID " + getSubjectID(n));
      LogFileCombined.print(") is player " + usersSlot);
      LogFileCombined.print(" in Game " + usersGame);
      LogFileCombined.print(" with user " + Trial.getUsersOpponent(n));
      
      LogFileCombined.print(" (Subject ID " + getSubjectID(Trial.getUsersOpponent(n)) + ")");
      if(usersSlot == 0){
        LogFileCombined.print(" Width = "+Env[usersGame].Player0.playerWidth);
        LogFileCombined.print(" Starting position = "+Env[usersGame].Player0.playerPosition);
        LogFileCombined.print(" Lure offset = "+Env[usersGame].Player0.lureOffset);
        LogFileCombined.print(" Lure width = "+Env[usersGame].Player0.lureWidth);
        LogFileCombined.print(" Static object position = "+Env[usersGame].Player0.staticPos);
        LogFileCombined.print(" Static object width = "+Env[usersGame].Player0.staticWidth);
      } else{
        LogFileCombined.print(" Width = "+Env[usersGame].Player1.playerWidth);
        LogFileCombined.print(" Starting position = "+Env[usersGame].Player1.playerPosition);
        LogFileCombined.print(" Lure offset = "+Env[usersGame].Player1.lureOffset);
        LogFileCombined.print(" Lure width = "+Env[usersGame].Player1.lureWidth);
        LogFileCombined.print(" Static object position = "+Env[usersGame].Player1.staticPos);
        LogFileCombined.print(" Static object width = "+Env[usersGame].Player1.staticWidth);
      }
      LogFileCombined.println();
    }
    
    LogFileCombined.print("Step,");
    LogFileCombined.print("System Time,");
    LogFileCombined.print("Incrimental Time,");
    LogFileCombined.print("Target step time,");
    LogFileCombined.print("Last step time,");
    //for each user, print out the column headers
    
    for(int n = 0; n < numberOfUnits; n++){
      String usr = "User " + String.valueOf(n);
      LogFileCombined.print(usr + "Position,");
      LogFileCombined.print(usr + "Button,");
      LogFileCombined.print(usr + "LED,");
      LogFileCombined.print(usr + "VIB1,");
      LogFileCombined.print(usr + "VIB2,");
      LogFileCombined.print(usr + "Tracker X Change,");
      LogFileCombined.print(usr + "Tracker Y Change,");
      LogFileCombined.print(usr + "Elapsed Time,");
      LogFileCombined.print(usr + "Comm Errors,");
      LogFileCombined.print(usr + "Comm Timeouts,");
      LogFileCombined.print(usr + "Response Time,");
    }
    LogFileCombined.println();
  }
}


void logTrialData(){
  //save everything to the files

    //write round number
    
  
  if(logCombinedOpen == true){
    
    LogFileCombined.print(String.valueOf(roundStep) + ',');
    LogFileCombined.print(String.valueOf(System.currentTimeMillis()) + ',');
    LogFileCombined.print(String.valueOf(incrimentalTime) + ',');
    LogFileCombined.print(String.valueOf(millisecondsBetweenSteps) + ',');
    LogFileCombined.print(String.valueOf(lastStepTime) + ',');
    
    for(int n = 0; n < numberOfUnits; n++){
      int usersGame;
      int usersSlot;
      usersGame = Trial.getUsersGame(n);
      usersSlot = Trial.getPlayerSlot(n);
      if(usersSlot == 0){
        LogFileCombined.print(String.valueOf(Env[usersGame].Player0.playerPosition)     + ',');
        LogFileCombined.print(String.valueOf(Env[usersGame].Player0.buttonState)         + ',');
        LogFileCombined.print(String.valueOf(Env[usersGame].Player0.LEDBrightness)         + ',');
        LogFileCombined.print(String.valueOf(Env[usersGame].Player0.hapticStrength0)     + ',');
        LogFileCombined.print(String.valueOf(Env[usersGame].Player0.hapticStrength1)     + ',');
        LogFileCombined.print(String.valueOf(Env[usersGame].Player0.XChange)   + ',');
        LogFileCombined.print(String.valueOf(Env[usersGame].Player0.XChange) + ',');
        LogFileCombined.print(String.valueOf(User[n].elapsedTime) + ',');
        LogFileCombined.print(String.valueOf(User[n].errorCount) + ',');
        LogFileCombined.print(String.valueOf(User[n].timeouts) + ',');
        LogFileCombined.print(String.valueOf(User[n].responseTime) + ',');
      }else{
        LogFileCombined.print(String.valueOf(Env[usersGame].Player1.playerPosition)     + ',');
        LogFileCombined.print(String.valueOf(Env[usersGame].Player1.buttonState)         + ',');
        LogFileCombined.print(String.valueOf(Env[usersGame].Player1.LEDBrightness)         + ',');
        LogFileCombined.print(String.valueOf(Env[usersGame].Player1.hapticStrength0)     + ',');
        LogFileCombined.print(String.valueOf(Env[usersGame].Player1.hapticStrength1)     + ',');
        LogFileCombined.print(String.valueOf(Env[usersGame].Player1.XChange)   + ',');
        LogFileCombined.print(String.valueOf(Env[usersGame].Player1.XChange) + ',');
        LogFileCombined.print(String.valueOf(User[n].elapsedTime) + ',');
        LogFileCombined.print(String.valueOf(User[n].errorCount) + ',');
        LogFileCombined.print(String.valueOf(User[n].timeouts) + ',');
        LogFileCombined.print(String.valueOf(User[n].responseTime) + ',');
      }
    }
    if(logBenchmarks){
      LogFileCombined.print(String.valueOf(loopStartMark) + ',');
      LogFileCombined.print(String.valueOf(loopMark1-loopStartMark) + ',');
      LogFileCombined.print(String.valueOf(loopMark2-loopStartMark) + ',');
      LogFileCombined.print(String.valueOf(loopMark3-loopStartMark) + ',');
      LogFileCombined.print(String.valueOf(loopMark4-loopStartMark) + ',');
      LogFileCombined.println(String.valueOf(loopEndMark-loopStartMark));
    }
    else  LogFileCombined.println();
  }
  
  
  
      //LogFileCombined.print(String.valueOf(boolToInt(Env[0].Player0SelfOverlap)) + ',');
      //LogFileCombined.print(String.valueOf(boolToInt(Env[0].Player1SelfOverlap)) + ',');
      //LogFileCombined.print(String.valueOf(boolToInt(Env[0].Player0OverlapLure1)) + ',');
      //LogFileCombined.print(String.valueOf(boolToInt(Env[0].Player1OverlapLure0)) + ',');
      //LogFileCombined.print(String.valueOf(boolToInt(Env[0].bothUsersTouching)) + ',');
      //LogFileCombined.print(String.valueOf(errorCount)           + ','); 
     // LogFileCombined.print(String.valueOf(timeouts)             + ',');
      //LogFileCombined.print(String.valueOf(responseTime)         + ','); //time between sending a data request and getting a reply
      //LogFileCombined.print(String.valueOf(Env[0].Player0.userPosition)    + ',');
}


void startLogs(){
  //startLogA();
  //startLogB();
  startLogCombined();
}

void stopLogs(){
  //stopLogA();
  //stopLogB();
  stopLogCombined();
}
void startLogCombined(){
  //Open a new file wigh the name bLog and appended with the date
  if(logCombinedOpen == true) {
    println("Error combined log already open");
    return;
  }
  String filedate = hour() + "," + minute() + "," + second() + " " + day() + "-" + month() + "-" + year();
  String filename = "PCE_COMBINED_LOG " + filedate + ".csv";
  println("Starting combined log");
  LogFileCombined = createWriter(filename);
  logCombinedOpen = true;
  writeFileHeader();
}

void stopLogCombined(){
  //close the file
  if(logCombinedOpen == false) {
    println("Error combined log already closed");
    return;
  }
  println("Stopping combined log");
  LogFileCombined.flush();  // Writes the remaining data to the file
  LogFileCombined.close();
  logCombinedOpen = false;
}

void startLogA(){
  //Open a new file wigh the name bLog and appended with the date
  if(logAOpen == true) {
    println("Error Log A already open");
    return;
  }
  String filedate = hour() + "," + minute() + "," + second() + " " + day() + "-" + month() + "-" + year();
  String filename = "PCE_LOGA " + filedate + ".csv";
  println("Starting Log A");
  LogFileA = createWriter(filename);
  logAOpen = true;
}


void stopLogA(){
  //close the file
  if(logAOpen == false) {
    println("Error Log A already closed");
    return;
  }
  println("Stopping Log A");
  LogFileA.flush();  // Writes the remaining data to the file
  LogFileA.close();
  logAOpen = false;
}

void startLogB(){
  //Open a new file wigh the name bLog and appended with the date
  if(logBOpen == true) {
    println("Error Log B already open");
    return;
  }
  String filedate = hour() + "," + minute() + "," + second() + " " + day() + "-" + month() + "-" + year();
  String filename = "PCE_LOGA " + filedate + ".csv";
  println("Starting Log B");
  LogFileB = createWriter(filename);
  logBOpen = true;
}

void stopLogB(){
  //close the file
  if(logBOpen == false) {
    println("Error Log B already closed");
    return;
  }
  println("Stopping Log B");
  LogFileB.flush();  // Writes the remaining data to the file
  LogFileB.close();
  logBOpen = false;
}

void logDataCombined(String dataLine){
   if(logCombinedOpen == false) return;
   LogFileCombined.append(dataLine);
}

void logDataA(String dataLine){
   if(logAOpen == false) return;
   LogFileA.append(dataLine);
}

void logDataB(String dataLine){
   if(logBOpen == false) return;
   LogFileB.append(dataLine);
}
