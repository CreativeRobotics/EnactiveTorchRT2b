
void pauseTrackerball(){
  trackerPaused = true;
}
void resumeTrackerball(){
  trackerPaused = false;
}

void startTrackerball(bool swap, int8_t xDir, int8_t yDir){
  trackXDirection = xDir;
  trackYDirection = yDir;
  swapXY = swap;
  if(swapXY){
    //don't be stupid - need to change the pins as well . . .
    //Create ISR's for inverted operation
    attachInterrupt(TRACKERBALL_XA, chA_XY_ISR, CHANGE);
    attachInterrupt(TRACKERBALL_XB, chB_XY_ISR, CHANGE);
    attachInterrupt(TRACKERBALL_YA, chA_YX_ISR, CHANGE);
    attachInterrupt(TRACKERBALL_YB, chB_YX_ISR, CHANGE);
  }else{
    attachInterrupt(TRACKERBALL_XA, chA_X_ISR, CHANGE);
    attachInterrupt(TRACKERBALL_XB, chB_X_ISR, CHANGE);
    attachInterrupt(TRACKERBALL_YA, chA_Y_ISR, CHANGE);
    attachInterrupt(TRACKERBALL_YB, chB_Y_ISR, CHANGE);
  }
}

void stopTrackerball(){
  detachInterrupt(TRACKERBALL_XA);
  detachInterrupt(TRACKERBALL_XB);
  detachInterrupt(TRACKERBALL_YA);
  detachInterrupt(TRACKERBALL_YB);

}

void resetTrackerball(){
  xCountTotal = 0;
  yCountTotal = 0;
}
void chA_X_ISR(){
  if(trackerPaused)return;
  boolean stateA = 0, stateB = 0;
  stateA = digitalRead(TRACKERBALL_XA);
  stateB = digitalRead(TRACKERBALL_XB);
  if(stateA && stateB)        xCountTotal-=trackXDirection;
  else if(stateA && !stateB)  xCountTotal+=trackXDirection;
  else if(!stateA && stateB)  xCountTotal+=trackXDirection;
  else if(!stateA && !stateB) xCountTotal-=trackXDirection;
  //USB.println(leftCountTotal);
  return;
}

//----------------------------------------------------
void chB_X_ISR(){
  if(trackerPaused)return;
  boolean stateA = 0, stateB = 0;
  stateA = digitalRead(TRACKERBALL_XA);
  stateB = digitalRead(TRACKERBALL_XB);
  if(stateA && stateB)        xCountTotal+=trackXDirection;
  else if(stateA && !stateB)  xCountTotal-=trackXDirection;
  else if(!stateA && stateB)  xCountTotal-=trackXDirection;
  else if(!stateA && !stateB) xCountTotal+=trackXDirection;
  //USB.println(leftCountTotal);
  return;
}

//Swapped functions - uses X axis to incriment Y values
void chA_XY_ISR(){
  if(trackerPaused)return;
  boolean stateA = 0, stateB = 0;
  stateA = digitalRead(TRACKERBALL_XA);
  stateB = digitalRead(TRACKERBALL_XB);
  if(stateA && stateB)        yCountTotal-=trackYDirection;
  else if(stateA && !stateB)  yCountTotal+=trackYDirection;
  else if(!stateA && stateB)  yCountTotal+=trackYDirection;
  else if(!stateA && !stateB) yCountTotal-=trackYDirection;
  //USB.println(leftCountTotal);
  return;
}

//----------------------------------------------------
void chB_XY_ISR(){
  if(trackerPaused)return;
  boolean stateA = 0, stateB = 0;
  stateA = digitalRead(TRACKERBALL_XA);
  stateB = digitalRead(TRACKERBALL_XB);
  if(stateA && stateB)        yCountTotal+=trackYDirection;
  else if(stateA && !stateB)  yCountTotal-=trackYDirection;
  else if(!stateA && stateB)  yCountTotal-=trackYDirection;
  else if(!stateA && !stateB) yCountTotal+=trackYDirection;
  //USB.println(leftCountTotal);
  return;
}

void chA_Y_ISR(){
  if(trackerPaused)return;
  boolean stateA = 0, stateB = 0;
  stateA = digitalRead(TRACKERBALL_YA);
  stateB = digitalRead(TRACKERBALL_YB);
  if(stateA && stateB)        yCountTotal+=trackYDirection;
  else if(stateA && !stateB)  yCountTotal-=trackYDirection;
  else if(!stateA && stateB)  yCountTotal-=trackYDirection;
  else if(!stateA && !stateB) yCountTotal+=trackYDirection;
  //USB.println(leftCountTotal);
  return;
}

//----------------------------------------------------
void chB_Y_ISR(){
  if(trackerPaused)return;
  boolean stateA = 0, stateB = 0;
  stateA = digitalRead(TRACKERBALL_YA);
  stateB = digitalRead(TRACKERBALL_YB);
  if(stateA && stateB)        yCountTotal-=trackYDirection;
  else if(stateA && !stateB)  yCountTotal+=trackYDirection;
  else if(!stateA && stateB)  yCountTotal+=trackYDirection;
  else if(!stateA && !stateB) yCountTotal-=trackYDirection;
  //USB.println(leftCountTotal);
  return;
}

//Swapped function - Uses Y axis to incriment X values

void chA_YX_ISR(){
  if(trackerPaused)return;
  boolean stateA = 0, stateB = 0;
  stateA = digitalRead(TRACKERBALL_YA);
  stateB = digitalRead(TRACKERBALL_YB);
  if(stateA && stateB)        xCountTotal+=trackXDirection;
  else if(stateA && !stateB)  xCountTotal-=trackXDirection;
  else if(!stateA && stateB)  xCountTotal-=trackXDirection;
  else if(!stateA && !stateB) xCountTotal+=trackXDirection;
  //USB.println(leftCountTotal);
  return;
}

//----------------------------------------------------
void chB_YX_ISR(){
  if(trackerPaused)return;
  boolean stateA = 0, stateB = 0;
  stateA = digitalRead(TRACKERBALL_YA);
  stateB = digitalRead(TRACKERBALL_YB);
  if(stateA && stateB)        xCountTotal-=trackXDirection;
  else if(stateA && !stateB)  xCountTotal+=trackXDirection;
  else if(!stateA && stateB)  xCountTotal+=trackXDirection;
  else if(!stateA && !stateB) xCountTotal-=trackXDirection;
  //USB.println(leftCountTotal);
  return;
}
