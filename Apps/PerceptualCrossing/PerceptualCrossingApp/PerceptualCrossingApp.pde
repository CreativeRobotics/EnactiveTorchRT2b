//Perceptual Crossing Experiment

import controlP5.*;
import processing.serial.*;

import java.util.*;
//userObject Player0;


//userObject Player1;


PCEnvironment[] Env;
PCInterfaceUnit[] User;
TrialController Trial;
int test1 = 0;
long timeTest = 0;
int numberOfEnvironments =1; //CHANGE THIS TO USE MULTIPLE VIRTUAL ENVIRONMENTS
int numberOfUnits = numberOfEnvironments*2;
int environmentWidth = 600;

void setup()
{
  
  size(1280, 700);//, P2D);
  preSetup();
  numberOfUnits = numberOfEnvironments*2;
  
  User = new PCInterfaceUnit[numberOfUnits];
  for(int n = 0; n < numberOfUnits; n++){
    User[n] = new PCInterfaceUnit(n);
    //User[n].unitNumber = n;
  }
  
  environmentStart = (width/2) - (environmentWidth/2); //The distance from the screen edge that tne environment starts
  envYPos = height-200; //environment Y Position onscreen
  Env = new PCEnvironment[numberOfEnvironments];
  for(int n = 0; n < numberOfEnvironments; n++){
    Env[n] = new PCEnvironment(environmentWidth);
  }
  Trial = new TrialController(2);
  Trial.initialiseMapping();
  Trial.setMapping(0);
  /*for(int n = 0; n < Trial.numberOfGames; n++){
    println("Game number " + String.valueOf(n) + " has User " + String.valueOf(Trial.playerPairs[n][0]) + " with User " + String.valueOf(Trial.playerPairs[n][1]));
  }
  for(int x = 0; x < 3; x++){
    Trial.setMapping(x);
    println("Setting Map ..." + String.valueOf(x));
  
    for(int n = 0; n < Trial.numberOfGames; n++){
      println("Game number " + String.valueOf(n) + " has User " + String.valueOf(Trial.playerPairs[n][0]) + " with User " + String.valueOf(Trial.playerPairs[n][1]));
      
      println("In game " + String.valueOf(n) + " Player 0 is user " + Trial.getPlayer0User(n));
      println("In game " + String.valueOf(n) + " Player 1 is user " + Trial.getPlayer1User(n));
    }
  }*/
  
  setupTabs();
  initialiseControls();
  initConfig();
  frameRate(systemFrameRate);
  
  //randomisePlayerPositions();
  //randomiseDimensions();
  resetEnvironments();
  initEnvironments(player0StaticObjectPosition, player1StaticObjectPosition); 
  drawGraphics();
}

void draw()
{
  if(Trial.trialMode == TRIAL_ACTIVE && Trial.roundActive == false){
    background(backgroundColTRIAL_ACTIVE);
    fill(tabBackgroundColTRIAL_ACTIVE);
  }
  else if(Trial.trialMode == TRIAL_ACTIVE && Trial.roundActive == true){
    background(backgroundColROUND_ACTIVE);
    fill(tabBackgroundColROUND_ACTIVE);
  }
  else{
    background(backgroundCol);
    fill(tabBackgroundCol);
  }
  rect(0,0,width,30);
  if(MOUSE_TEST_MODE == true) updateMouseMode();
  
  if(Trial.trialMode == MANUAL) updateManual();
  else if(Trial.trialMode == FREE_PRACTICE) updateFreePractice();
  else if(Trial.trialMode == TRIAL_ACTIVE){ //trial active
    //Do nothing untol roundActive is true and countdown is 0
    if(Trial.roundActive == false) updateFreePractice();
    if(Trial.roundActive == true && countdown == 0) {
      //halt the loop and stop drawing on the screen
      //Any keyboard input now will hald the trial
      noLoop();
      runRound(); //run one trial
      //runTrial only returns at the end of the trial
      loop();
      endRound();
    }
    if(Trial.roundActive == true && countdown > 0){
      countdown--;
      println(countdown);
      //redraw();
      delay(1);
    }
  }
  
}


void drawGraphics(){
  int vSpacing = envHeight*2;
  drawEnviron(envYPos, vSpacing);  
  drawUsers(envYPos, vSpacing);
  drawCollisions(envYPos, vSpacing);
  stroke(0);
  fill(0);
}

void drawUsers(int yPos, int spacing ){
  int yPosition = yPos;
  for(int n = 0; n < numberOfEnvironments; n++){
    drawPlayer0(yPosition-20, Env[n].Player0 );
    drawPlayer1(yPosition+1,  Env[n].Player1 );
    yPosition += spacing;
  }
}
void drawEnviron(int yPos, int spacing){
    rectMode(CORNER);
    stroke(0);
    int yPosition = yPos;
    fill(255);  // Set fill to userCol
    for(int n = 0; n < numberOfEnvironments; n++){
      rect(environmentStart, yPosition-(envHeight/2), environmentWidth, envHeight );
      yPosition += spacing;
    }
}


void drawCollisions(int yPos, int spacing ){
  int yPosition = yPos;
  for(int n = 0; n < numberOfEnvironments; n++){
    drawCollissionIndicator(Env[n].Player0SelfOverlap, Env[n].Player0OverlapLure1, Env[n].bothUsersTouching, Env[n].Player0.displayHue, yPosition, -10);
    drawCollissionIndicator(Env[n].Player1SelfOverlap, Env[n].Player1OverlapLure0, Env[n].bothUsersTouching, Env[n].Player1.displayHue, yPosition, 10);
    yPosition += spacing;
  }
}

void drawCollissionIndicator(boolean flag1, boolean flag2, boolean flag3, int userHue, int yPos, int yOffset){
  //YOffset tells you what side to draw - top (-) or bottom (+)
  int centre = environmentStart+(environmentWidth/2);
  int start = environmentStart;
  int end = environmentStart+environmentWidth;
  int yStart = yOffset;
  int yEnd = yOffset;
  int indicatorWidth = 100;
  if(yOffset < 0){
    yStart = yPos-(envHeight/2);
    yEnd = -10;
  }   else{
    yStart = yPos+(envHeight/2);
    yEnd = 10;
  }
  
  if(flag1)fill(color(userHue, 150, staticBrightness)); //Static contact
  else fill(color(userHue, 10, 250));
  rect(start, yStart, indicatorWidth, yEnd);
  
  if(flag2) fill(color(userHue, 150, lureBrightness));//Lure contact
  else      fill(color(userHue, 10, 250));
  rect(start+indicatorWidth, yStart, indicatorWidth, yEnd);
 
  if(flag3) fill(color(userHue, 150, Playerbrightness));//User contact
  else fill(color(userHue, 10, 250));
  rect(start+indicatorWidth+indicatorWidth, yStart, indicatorWidth, yEnd);
}





void drawPlayer0(int offset, PCPlayer userObj){
  display(offset, userObj);
}

void drawPlayer1(int offset, PCPlayer userObj){
  display(offset, userObj);
}


void display(int yPos, PCPlayer userObj) {
    //brightnessDrop
    //draw on the screen using the yPos
    rectMode(CORNER);
    stroke(128);
    //rectMode(CENTER);
    stroke(0);
    fill(color(userObj.displayHue, 100, 200));  // Set fill to userCol
    //draw the static object
    drawRectWrap(environmentStart+userObj.staticPos, yPos, userObj.staticWidth, userHeight);
    //draw the users position
    fill(color(userObj.displayHue, 255, 150));
    drawRectWrap(environmentStart+userObj.playerPosition, yPos, userObj.playerWidth, userHeight);
    //draw the lure
    fill(color(userObj.displayHue, 150, 100));
    drawRectWrap(environmentStart+userObj.lurePosition, yPos, userObj.lureWidth, userHeight);
    //stroke(userCol);
    drawLineWrap(environmentStart+userObj.playerPosition+userObj.playerWidth, userObj.lureOffset, yPos+(userHeight/2));
    //line(environmentStartX+userPosition+userWidth, yPos+(userHeight/2), environmentStartX+lurePosition, yPos+(userHeight/2));
}
//-----------------------------------------------------------------------
void drawRectWrap(int xS, int yS, int xWidth, int yHeight){
  //draw a rectangle but make sure it wraps around the world edges
  int worldEnd = environmentWidth+environmentStart; //compensate for the offset
  //println(worldEnd);
  rectMode(CORNER);
  if(xS+xWidth < worldEnd && xS >= environmentStart){
    //normal
    rect(xS, yS, xWidth, yHeight);
  }
  else{
    //split the rectangle in half and draw one at each end
    rect(xS, yS, worldEnd-xS, yHeight);
    rect(environmentStart, yS, (xS+xWidth)-worldEnd, yHeight);
  }
}
//-----------------------------------------------------------------------
void drawLineWrap(int xS, int lLength, int yHeight){
  //draw a line that wraps
  int worldEnd = environmentWidth+environmentStart; //compensate for the offset
  //println(worldEnd);
  
  if(xS+lLength < worldEnd && xS >= environmentStart){
    line(xS, yHeight, xS+lLength, yHeight);
  }
  else{
    if(xS < worldEnd){
      line(xS, yHeight, worldEnd, yHeight);
      line(environmentStart, yHeight, environmentStart+(lLength-(worldEnd-xS)), yHeight);
    }
    else line(environmentStart+(xS-worldEnd), yHeight, environmentStart+(lLength-(worldEnd-xS)), yHeight);
  }
}
  
  
