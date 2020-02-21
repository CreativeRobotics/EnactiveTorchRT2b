
void updateManual(){
 //do nothing 
 drawGraphics();
  stroke(0);
  fill(0);
  practiceIterations++;
}
//=====================================================================================================================================================================

void updateFreePractice(){
  if( millis() < lastFreePracticeUpdateTime+freePracticeUpdateInterval) return;
  boolean printTimeTest = false;
  boolean printPracticeDebugMessages = false;
  if(NOUnitsConnected()) {
    
    drawGraphics();
    return;
  }
  for(int n = 0; n < numberOfUnits; n++){
    //check each serial IO unit for new data
    //If no new data then request new data
    if(User[n].waitingForReply == false && User[n].portConnected == true){
      timeTest = System.nanoTime();
      //if(practiceIterations % messageFrequency == 0){ //only send countdown if the start message has timed out
        //println("Practice:" + practiceIterations);
        //User[n].getStateMessage("Practice:" +practiceIterations);
      //}
      //else User[n].getState();
      User[n].getState();
      if(printPracticeDebugMessages) println("Sent command to unit " + String.valueOf(n));
    }
    //Now wait for all the units to reply
  } 
  if(waitForReply(freePracticeTimeout)){
    replyTimeouts++;//wait for reply with 50ms timeout, incriment the counter if it returns a timeout error
    println("Timeout " + replyTimeouts);
  }
  
  for(int n = 0; n < numberOfEnvironments; n++){
    updateEnvironment(n);
  }
  setHaptics();
  drawGraphics();
  practiceIterations++;
}
//=====================================================================================================================================================================
void updateMouseMode(){
  int xmPos = -( (width/2) - mouseX);
  
  for(int n = 0; n < numberOfEnvironments; n++){
    if (mousePressed == true) {
      //println("Mouse Value = " + String.valueOf(xmPos) + " Wrap = " + Env[n].wrap(xmPos));
     if (mouseButton == LEFT) Env[n].Player0.move(xmPos);
     if (mouseButton == RIGHT) Env[n].Player1.move(xmPos);
    }
    updateEnvironment(n);
  }
  drawGraphics();
  practiceIterations++;
}
//=====================================================================================================================================================================
void resetEnvironments(){
  for(int n = 0; n < numberOfEnvironments; n++){
    Env[n].Player0.setUserParameters(usersWidth, luresWidth, gapToLure, staticObjectWidth, player0StaticObjectPosition);
    Env[n].Player1.setUserParameters(usersWidth, luresWidth, gapToLure, staticObjectWidth, player1StaticObjectPosition);
  }
}
void randomisePlayerPositions(){
    //Setup random user positions:
  for(int n = 0; n < numberOfEnvironments; n++){
    Env[n].Player0.playerPosition = int(random(0,environmentWidth));
    Env[n].Player1.playerPosition = int(random(0,environmentWidth));
    Env[n].Player0.move(Env[n].Player0.playerPosition);
    Env[n].Player1.move(Env[n].Player1.playerPosition);
  }
}
void randomiseStaticPositions(){
    //Setup random user positions:
  for(int n = 0; n < numberOfEnvironments; n++){
    Env[n].Player0.setStaticParameters(Env[n].Player0.staticWidth , int(random(environmentWidth))); //width, position
    Env[n].Player1.setStaticParameters(Env[n].Player1.staticWidth , int(random(environmentWidth)));
  }
}

void randomiseDimensions(){//Setup random user sizes:
  for(int n = 0; n < numberOfEnvironments; n++){
    Env[n].Player0.setUserParameters(int(random(2,16)), int(random(2,16)), int(random(10, 400)), int(random(2,16)), Env[n].Player0.staticPos);
    Env[n].Player1.setUserParameters(int(random(2,16)), int(random(2,16)), int(random(10, 400)), int(random(2,16)), Env[n].Player0.staticPos);
    
    Env[n].Player0.move(int(random(environmentWidth)));
    Env[n].Player1.move(int(random(environmentWidth)));
    Env[n].Player0.setStaticParameters(int(random(2,16)), Env[n].Player0.staticPos); //width, position
    Env[n].Player1.setStaticParameters(int(random(2,16)), Env[n].Player1.staticPos);
  }
}
