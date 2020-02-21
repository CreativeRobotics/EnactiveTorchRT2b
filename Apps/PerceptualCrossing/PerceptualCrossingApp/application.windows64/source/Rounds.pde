


void runRound(){
  //Do everything here
  long startMessageExpiry = Trial.startMessageTime;
  long endMessageExpiry = Trial.endMessageTime;
  boolean printRoundDebug = false;
  
  String starterMessage = cp5.get(Textfield.class, "StartRoundTextbox").getText();
  String enderMessage   = cp5.get(Textfield.class, "EndRoundTextbox").getText(); 
  //get the trial beep data from the UI
  updateTimingData();
  
  incrimentalTime = 0;
  numberOfIterations = roundLengthSeconds * roundResolution;
  millisecondsBetweenSteps = 1000/roundResolution;
  for(int n = 0; n < numberOfUnits; n++){
    User[n].timeouts = 0;
  }
  roundStep = 0;
  initEnvironments(player0StaticObjectPosition, player1StaticObjectPosition); //randomise user positions and set static object positions
  if(noUnitTest == true) {
    //delay then return;
    delay(1000);
    return;
  }
  
  //Add a half second beep before starting
  
  /*
  for(int n = 0; n < numberOfUnits; n++)User[n].sendBeepOn();
  delay(500);
  for(int n = 0; n < numberOfUnits; n++) User[n].sendBeepOff();
  */
  
  roundStartTime = System.currentTimeMillis();
  startMessageExpiry += roundStartTime;
  sendStartRound(starterMessage);
  trialMarkON();
  Trial.startIndicatorOn = true;
  
  for(int step = 0; step < numberOfIterations; step++){
    if(logBenchmarks) loopStartMark = System.currentTimeMillis();
    lastStepStart = System.currentTimeMillis();
    //println(endOfStep-lastStepStart);
    if(checkExitKey()) return;
    
    if(logBenchmarks) loopMark1 = System.currentTimeMillis();
    
    updateIntervals(); //update any events that need to happen during the trial
    
    //Send requests for data to each unit
    for(int n = 0; n < numberOfUnits; n++){
      //Reset each units change variables so that if we get a timeout the old values will not be used
      User[n].resetUserChange();
      User[n].sentTime = System.currentTimeMillis();
      if(roundStep % messageFrequency == 0 && System.currentTimeMillis() > startMessageExpiry){ //only send countdown if the start message has timed out
        String message = String.valueOf((numberOfIterations-roundStep)/100);
        println(message);
        User[n].getStateMessage(message);
      }
      else User[n].getState();
    }
    //wait for reply with a timeout
    if(waitForReply(9) == true) replyTimeouts++;;
    if(logBenchmarks) loopMark2 = System.currentTimeMillis();
    
    /*for(int n = 0; n < numberOfUnits; n++){
      if(User[n].newData) {
        User[n].responseTime = System.currentTimeMillis()-User[n].sentTime;
        applymovement(n); //an ugly hack only works with four units
        User[n].newData = false;
      }
    }*/
    //Then update the trial
    if(logBenchmarks) loopMark3 = System.currentTimeMillis();
    for(int n = 0; n < numberOfEnvironments; n++) updateEnvironment(n);
    setHaptics();
    if(logBenchmarks) loopMark4 = System.currentTimeMillis();
    if(animateRounds) drawGraphics();
    
    while( System.currentTimeMillis() < lastStepStart+millisecondsBetweenSteps){
      delay(1);
    } //wait here until it is time for the next iteration
    incrimentalTime += lastStepTime;
    //work out exactly how much time elapsed just in case it was more than the required timestep
    lastStepTime = System.currentTimeMillis() - lastStepStart;
    //finally log the data
    if(logBenchmarks) loopEndMark = System.currentTimeMillis();
    logTrialData();
    roundStep++;
  }
  sendEndRoundMessage(enderMessage);
  endMessageExpiry += System.currentTimeMillis();
  
  trialMarkON(); //beep and flash
  delay((int)Trial.endIndicatorLength);
  trialMarkOFF();
  while(System.currentTimeMillis() < endMessageExpiry);
  sendHaltRoundAll(false);
  for(int n = 0; n < numberOfUnits; n++)User[n].resetVariables();
  resetEnvironments();
  
  println("Ended Round");
}
//=====================================================================================================================================================================
//beep and flash
void trialMarkON(){
  if(Trial.useBeepMark == true && Trial.useLEDMark == true) ledBeepOn();
  else if(Trial.useLEDMark == true) LEDsOn();
  else if(Trial.useBeepMark == true) beepsOn();
}
//=====================================================================================================================================================================
void trialMarkOFF(){
  if(Trial.useBeepMark == true && Trial.useLEDMark == true) ledBeepOff();
  else if(Trial.useLEDMark == true) LEDsOff();
  else if(Trial.useBeepMark == true) beepsOff();
}

//=====================================================================================================================================================================

void updateIntervals(){
  //Send any light and beep commands
  //turn off it starting the trial and beep has run out of time
  if(Trial.startIndicatorOn == true && System.currentTimeMillis() >= (roundStartTime + Trial.startindicatorLength) ){
    trialMarkOFF();
    Trial.startIndicatorOn = false;
  }
  //beep/flash ON if you reach the middle marker and have not completed it
  else if(Trial.middleIndicatorOn == false && Trial.middleIndicatorDone == false && System.currentTimeMillis() >= (roundStartTime + Trial.middleIndicatorTime) ){
    //ledBeepOn();
    if(Trial.middleIndicatorLEDEnabled == true) LEDsOn();
    if(Trial.middleIndicatorBeepEnabled == true) beepsOn();
    Trial.middleIndicatorOn = true;
  }
  else if(Trial.middleIndicatorOn == true && System.currentTimeMillis() >= (roundStartTime + Trial.middleIndicatorTime + Trial.middleIndicatorLength) ){
    trialMarkOFF();
    Trial.middleIndicatorDone = true;
    Trial.middleIndicatorOn = false;
  }
}
//=====================================================================================================================================================================

void initEnvironments(int staticPos0, int staticPos1){
  //randomise positions
  for(int n = 0; n < numberOfEnvironments; n++){
    //Setup random user positions:
    Env[n].Player0.playerPosition = int(random(0,environmentWidth));
    Env[n].Player1.playerPosition = int(random(0,environmentWidth));
    Env[n].Player0.move(Env[n].Player0.playerPosition);
    Env[n].Player1.move(Env[n].Player1.playerPosition);
    //reset the static object positions
    
    Env[n].Player0.setStaticParameters(staticObjectWidth, staticPos0);
    Env[n].Player1.setStaticParameters(staticObjectWidth, staticPos1);
  }
}
//=====================================================================================================================================================================

boolean waitForReply(int timeout){
  //wait for a reply
  boolean returnError = false;
  int timeoutTimer = 0;
  for(int n = 0; n < numberOfUnits; n++){
      if( User[n].surpress )  User[n].waitingForReply = false;
  }
  /*
  check each unit for a reply, if any unit is still waiting incriment the
  */
  int replies = 0;
  while(true){
    replies = numberOfUnits;
    for(int n = 0; n < numberOfUnits; n++){
      if(User[n].waitingForReply == false) replies--;
    }
    if(replies < 1) return false; //all replied within the time, return no error
    timeoutTimer++;
    delay(1);
    if(timeoutTimer > timeout){ //reset any units that have not had a reply
      returnError = true;
      for(int n = 0; n < numberOfUnits; n++) if(User[n].waitingForReply == true)User[n].resetTimeout();
      return true;
    }
  }
  //return true;
  /*
  while(checkReplyStatusAll()){
    timeoutTimer++;
    delay(1);
    if(timeoutTimer > timeout){
      for(int n = 0; n < numberOfUnits; n++){
        User[n].newData = false;
        User[n].waitingForReply = false;
        User[n].timeouts++;
      }
      return true;
    }
  }
  return false; //no error, no timeout*/
}
//=====================================================================================================================================================================

boolean checkReplyStatusAll(){
  //return true if any unit is waiting for a reply
  int replies = 0;
 for(int n = 0; n < numberOfUnits; n++){
   if( User[n].waitingForReply == true  && User[n].portConnected == true) replies++;
 }
 if(replies > 0) return true;
 return false;
}

//=====================================================================================================================================================================
