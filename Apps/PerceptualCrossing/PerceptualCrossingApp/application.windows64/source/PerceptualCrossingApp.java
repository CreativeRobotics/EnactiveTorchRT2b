import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import controlP5.*; 
import processing.serial.*; 
import java.util.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class PerceptualCrossingApp extends PApplet {

//Perceptual Crossing Experiment





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

public void setup()
{
  
  //, P2D);
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

public void draw()
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


public void drawGraphics(){
  int vSpacing = envHeight*2;
  drawEnviron(envYPos, vSpacing);  
  drawUsers(envYPos, vSpacing);
  drawCollisions(envYPos, vSpacing);
  stroke(0);
  fill(0);
}

public void drawUsers(int yPos, int spacing ){
  int yPosition = yPos;
  for(int n = 0; n < numberOfEnvironments; n++){
    drawPlayer0(yPosition-20, Env[n].Player0 );
    drawPlayer1(yPosition+1,  Env[n].Player1 );
    yPosition += spacing;
  }
}
public void drawEnviron(int yPos, int spacing){
    rectMode(CORNER);
    stroke(0);
    int yPosition = yPos;
    fill(255);  // Set fill to userCol
    for(int n = 0; n < numberOfEnvironments; n++){
      rect(environmentStart, yPosition-(envHeight/2), environmentWidth, envHeight );
      yPosition += spacing;
    }
}


public void drawCollisions(int yPos, int spacing ){
  int yPosition = yPos;
  for(int n = 0; n < numberOfEnvironments; n++){
    drawCollissionIndicator(Env[n].Player0SelfOverlap, Env[n].Player0OverlapLure1, Env[n].bothUsersTouching, Env[n].Player0.displayHue, yPosition, -10);
    drawCollissionIndicator(Env[n].Player1SelfOverlap, Env[n].Player1OverlapLure0, Env[n].bothUsersTouching, Env[n].Player1.displayHue, yPosition, 10);
    yPosition += spacing;
  }
}

public void drawCollissionIndicator(boolean flag1, boolean flag2, boolean flag3, int userHue, int yPos, int yOffset){
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





public void drawPlayer0(int offset, PCPlayer userObj){
  display(offset, userObj);
}

public void drawPlayer1(int offset, PCPlayer userObj){
  display(offset, userObj);
}


public void display(int yPos, PCPlayer userObj) {
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
public void drawRectWrap(int xS, int yS, int xWidth, int yHeight){
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
public void drawLineWrap(int xS, int lLength, int yHeight){
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
  
  
//all global variables
boolean MOUSE_TEST_MODE = false;
boolean animateRounds = false;
int replyTimeouts = 0;

boolean noUnitTest = false;



long freePracticeUpdateInterval = 50;
long lastFreePracticeUpdateTime = 0;
int freePracticeTimeout = 120;


static final int countdownDefault = 1;
int countdown = countdownDefault; //used to create a pause before a round starts as a hack to ensure the screen redraws before the round starts
/*Practice modes:
0 = no practice
4 = free practice
*/
int backgroundCol = color(220);
int tabBackgroundCol = color(170);

int backgroundColTRIAL_ACTIVE = color(255, 197, 110);
int tabBackgroundColTRIAL_ACTIVE = color(50);

int backgroundColROUND_ACTIVE = color(247, 98, 98);
int tabBackgroundColROUND_ACTIVE = color(50);
int practiceIterations = 0;

//Time benchmarking variables for measuring latencies on the trial loop
boolean logBenchmarks = false;
long loopStartMark = 0;
long loopMark1 = 0;
long loopMark2 = 0;
long loopMark3 = 0;
long loopMark4 = 0;
long loopEndMark = 0;
long endOfThisStep = 0;

//Timing for the trial
long roundStartTime;
//These are all set when the applications runs.
int numberOfIterations = 100;
long millisecondsBetweenSteps = 10;
int roundStep = 0;
//int targetTimestep = 0;
long lastStepStart = 0;
long lastStepTime = 0;

long incrimentalTime = 0;

long elapsedRoundTime =0;
long startTime = 0;
long endTime = 0;
long oldTime = 0;

//boolean surpressA = false;
int systemFrameRate = 100;

PFont pfont;
ControlFont largeFont;

int buttonNormal             = color(128); //mid grey
int buttonMouseover          = color(200); //light grey
int buttonActive             = color(128,0,0); //mid red
int buttonActiveMouseover    = color(200,0,0); //bright red

PrintWriter LogFileA;
boolean logAOpen = false;
PrintWriter LogFileB;
boolean logBOpen = false;
PrintWriter LogFileCombined;
boolean logCombinedOpen = false;


PFont myFont;

ControlP5 cp5;

Textlabel serialTextA;
String[] portList; //the list of available serial ports

int listBoxWidth = 200;
int listBoxHeight = 100;

int Playerbrightness = 255;
int lureBrightness = 180;
int staticBrightness = 120;

int environmentStart = 100; //how far from the screen edge
int envYPos = 200;
int envHeight = 50;
int userHeight = 20;

int movingObjectPosA = 100;
int movingObjectPosB = 100;
int slowSpeed = 1; //speed of moving object for practice 2 (Pixels per timestep)
int fastSpeed = 3; //speed of moving object for practice 3
public void RandomPos(int theValue){
  randomisePlayerPositions();
}
public void RandomStatic(int theValue){
  randomiseStaticPositions();
}
public void RandomDim(int theValue){
  randomiseDimensions();
}
public void ResetEnv(int theValue){
  resetEnvironments();
}
public void StartRoundButton(int theValue){
  if(Trial.trialMode != TRIAL_ACTIVE) {
    println("Error - trial not active");
    return;
  }
  countdown = countdownDefault;
  startRound();
  //Trial.trialMode = TRIAL_ACTIVE;
  println("Starting Round");
  //println("Count at " + countdown);
  //if(Trial.roundActive == true)  println("Round Active");
  //if(Trial.roundActive == false)  println("Round NOT Active");
  redraw();
}

public void ShiftDiad(int theValue){
  //shift diad mappings
  Trial.setMapping( ++Trial.playerMapping );
  cp5.getController( "gameMapping" ).setValue(Trial.playerMapping);
  cp5.get(Textlabel.class,"DiadOption").setText((String)playerMapListText.get(Trial.playerMapping));
}


public void StartTrialButton(int theValue){
  if(Trial.trialMode != TRIAL_ACTIVE){
     println("Trial activated");
    //Start a new trial  - open the log and write the trial header
    startTrial();
  }else println("Error, Trial already activated");
}

public void StopTrialButton(int theValue){
  if(Trial.trialMode == TRIAL_ACTIVE){
     println("Trial Halted");
    //Stop a trial if it is running  - open the log and write the trial header
    stopTrial();
  }else println("Error, Trial already stopped");
}
public void FreePractice(int theValue){
  Trial.trialMode = 1;
  practiceIterations = 0;
  
  cp5.get(Textlabel.class,"trialStatusLabel").setText("Practice Mode");
  randomisePlayerPositions();
}
public void ManualMode(int theValue){
  Trial.trialMode = 0;
  practiceIterations = 0;
  
  cp5.get(Textlabel.class,"trialStatusLabel").setText("Manual Mode");
}

/*public void animation(boolean theValue){
  animateRounds = theValue;
  //if(theValue) println("Animation ON");
  //else println("Animation OFF");
}*/
public void MouseMode(boolean theValue){
  MOUSE_TEST_MODE = theValue;
  //if(theValue) println("Mousemode ON");
  //else println("Mousemode OFF");
}
public void SetEnvParam(int theValue){
  updateTimingData();
}

//Trial control variables - adjust these as you need to

//Trial start and end messages to be displayed on the LCD

String  startTrialMessage               = "Begin!";
int     startTrialMessageDuration       = 2500;
String  endTrialMessage                 = "Einde taak. Dankjewel!";
int     endTrialMessageDuration         = 5000;
String  trialAbortedMessage             = "Trial aborted";

//Dimensions of the players in the virtual environments
int usersWidth          = 4;
int gapToLure           = 150;
int luresWidth          = 4;
int staticObjectWidth   = 4;
int player0StaticObjectPosition = 150;
int player1StaticObjectPosition = 450;


int roundLengthSeconds  = 60; //the length of the round in seconds
int roundResolution     = 100; //The number of updates per second - the maximum is approximatly 500
int messageFrequency    = 100; //how many steps before updating the users screen message - If roundResolution is 100 steps per second then setting messageFrequency to 100 will update the screen once per second





public void initConfig(){
  Trial.startMessageTime           = startTrialMessageDuration;
  Trial.endMessageTime             = endTrialMessageDuration;
  Trial.startindicatorLength       = 1000;

  Trial.middleIndicatorLEDEnabled   = false;
  Trial.middleIndicatorBeepEnabled   = false;
  Trial.middleIndicatorTime       = 5000;
  Trial.middleIndicatorLength     = 500;
  Trial.endIndicatorLength        = 1000;

  Trial.useBeepMark = false; //true of start and end points are marked with a beep
  Trial.useLEDMark = true;   //true of start and end points are marked with a flash
  
  
}

//update functions for the environments

public void updateEnvironment(int envNo){
  int user0 = -1, user1 = -1;
  user0 = Trial.getPlayer0User(envNo);
  user1 = Trial.getPlayer1User(envNo);
  Env[envNo].update(User[user0], User[user1]); //updates the player positions AND writes haptic values to the user objects
}




/*
A portabe set of class files for the virtual environment and a pair of users
Consists of a user class and an environment class which contains users
*/

class PCPlayer 
{
  public
  int playerWidth; //  width
  int playerPosition; // rect xposition
  int lurePosition;
  int lureOffset;
  int lureWidth;
  int staticPos;
  int staticWidth;
  
  int worldW;
  //remote data storage for variables from devices
  int displayHue;
  int ElapsedTime = 0;
  int XChange = 0;
  int YChange = 0;
  int buttonState = 0;
  int LEDBrightness = 0;
  int hapticStrength0 = 0;
  int haptic0Default = 1023;
  int hapticStrength1 = 0;
  int haptic1Default = 1023;
  
  //boolean touchingStatic = false;
  //Constructor
  PCPlayer(int uWidth, int lWidth, int lOffset, int sWidth, int sPos, int envW) { 
    
    worldW = envW;
    setplayerWidth(uWidth);
    setLureWidth(lWidth);
    setLureOffset(lOffset);
    setStaticPosition(sPos);
    setStaticWidth(sWidth);
  }
  //-----------------------------------------------------------------------
  public void Start(int startPos){
    move(startPos);
  }
  
  //-----------------------------------------------------------------------
  
  public void setWorldWidth(int newWWidth){
    worldW = newWWidth;
    staticPos = wrap(staticPos);
    playerPosition = wrap(playerPosition);
  }
  
  //-----------------------------------------------------------------------
  public void setUserParameters(int uWidth, int lWidth, int lOffset, int sWidth, int sPos){
    setplayerWidth(uWidth);
    setLureWidth(lWidth);
    setLureOffset(lOffset);
    setStaticPosition(sPos);
    setStaticWidth(sWidth);
    move();
  }
  //-----------------------------------------------------------------------
  public void setStaticParameters(int sWidth, int sPos){
    setStaticPosition(sPos);
    setStaticWidth(sWidth);
    
  }
  //-----------------------------------------------------------------------
  public void setplayerWidth(int uWidth){
    playerWidth = uWidth;
  }
  //-----------------------------------------------------------------------
  public void setLureWidth(int lWidth){
    lureWidth = lWidth;
  }
  //-----------------------------------------------------------------------
  public void setLureOffset(int lOffset){
    lureOffset = lOffset;
  }
  //-----------------------------------------------------------------------
  public void setStaticPosition(int sPos){
    staticPos = wrap(sPos);
  }
  //-----------------------------------------------------------------------
  public void setStaticWidth(int sWidth){
    staticWidth = sWidth;
  }
  //-----------------------------------------------------------------------
  //impliment classes for returning positions based on a worldWidth argument ...
  public int getUserStart(){
    return wrap(playerPosition);
  }
  //-----------------------------------------------------------------------
  public int getUserEnd(){
    return wrap(playerPosition+playerWidth);
  }
  //-----------------------------------------------------------------------
  public int getLureStart(){
    return wrap(playerPosition+playerWidth+lureOffset);
  }
  //-----------------------------------------------------------------------
  public int getLureEnd(){
    return wrap(playerPosition+playerWidth+lureOffset+lureWidth);
  }
  //-----------------------------------------------------------------------
  public int getStaticStart(){
    return wrap(staticPos);
  }
  //-----------------------------------------------------------------------
  public int getStaticEnd(){
    return wrap(staticPos+staticWidth);
  }
  //-----------------------------------------------------------------------
  public boolean isTouchingStatic(){
    //return true if the user is overlapping the static object
    return checkSelfOverlap();
  }
  //-----------------------------------------------------------------------
  public boolean checkSelfOverlap() {
    //returns true if the user is on the static object
    //This does NOT work when the static object is wrapping around the environment
    //Check to see if they are NOT overlapping
    int userEnd = wrap(playerPosition+playerWidth);
    int staticEnd = wrap(staticPos+staticWidth);
    if(userEnd < playerPosition && staticEnd > staticPos){ //handle wrapping around case for user but not static object
      if(playerPosition > staticEnd && userEnd < staticPos) return false;
      return true;
    }
    if(userEnd > playerPosition && staticEnd < staticPos){ //handle wrapping around case for static object but NOT user
      if(playerPosition > staticEnd && userEnd < staticPos) return false;
      return true;
    }
    if(userEnd < playerPosition && staticEnd < staticPos){ //handle wrapping around case for both objects
      return true;
    }
    if(playerPosition > staticEnd || userEnd < staticPos) return false;
    return true;
  }
  //-----------------------------------------------------------------------
  //handle values that wrap around the world width
  public int wrap(int inPosition) {
    
    int ps = inPosition;
    if(ps < 0) return (worldW+ps)% worldW;
    return inPosition % worldW;
    
    /*
    //wrap around the world limit
    if(inPosition < 0) return -inPosition % worldW;
    return inPosition % worldW;*/
  }
  //-----------------------------------------------------------------------
  public void move() {
    playerPosition = wrap(playerPosition);
    lurePosition = wrap( playerPosition + playerWidth + lureOffset);
  }
  //-----------------------------------------------------------------------
  public void move(int newPos) {
    playerPosition = wrap(newPos);
    lurePosition = wrap( playerPosition + playerWidth + lureOffset);
  }
  public void shift(int newPos) {
    playerPosition = wrap(playerPosition+newPos);
    lurePosition = wrap( playerPosition + playerWidth + lureOffset);
  }
  
  public void setHapticValues(int hapticVal){
    hapticStrength0 = hapticVal;
    hapticStrength1 = hapticVal;
  }
}







//===========================================================================================
class PCEnvironment 
{
  public
  int worldWidth = 600;
  PCPlayer Player0;
  PCPlayer Player1;
  
  int nextUpdateForPlayer0 = 0;
  int nextUpdateForPlayer1 = 0;
  int defaultUWidth = 8;
  int defaultLWidth = 6;
  int defaultLOffset = 100;
  int defaultSWidth = 10;
  int defaultSPos = 300;
  boolean Player0SelfOverlap = false;
  boolean Player1SelfOverlap = false;
  boolean bothUsersTouching = false;
  boolean Player0OverlapLure1 = false;
  boolean Player1OverlapLure0 = false;
  
  boolean Player0Contact = false;
  boolean Player1Contact = false;
  int hapticContactValue = 1023;
  int hapticNoContactValue = 0;
  //Constructor
  PCEnvironment(int wWidth) { 
    worldWidth = wWidth;
    Player0 = new PCPlayer(defaultUWidth, defaultLWidth, defaultLOffset, defaultSWidth, defaultSPos, worldWidth);
    Player1 = new PCPlayer(defaultUWidth, defaultLWidth, defaultLOffset, defaultSWidth, defaultSPos, worldWidth);
    //set everything to default and let the user initialise the required settings later
  }
  //-----------------------------------------------------------------------
  public void start(int Player0StartPos, int Player1StartPos){
    Player0.Start(Player0StartPos);
    Player1.Start(Player1StartPos);
  }
  //-----------------------------------------------------------------------
  public void setWorldWidth(int newW){
    worldWidth = newW;
    Player0.setWorldWidth(worldWidth);
    Player1.setWorldWidth(worldWidth);
  }
  //-----------------------------------------------------------------------
  //move by directly setting the user positions
  public void updateMove(int newPosA, int newPosB){
    Player0.move(newPosA);
    Player1.move(newPosB);
    //update all the overlap checks
    checkCollisions();
    updateHaptics();
    //Player0SelfOverlap = Player0.isTouchingStatic(worldWidth);
    //Player1SelfOverlap = Player1.isTouchingStatic(worldWidth);
  }
  //-----------------------------------------------------------------------
  public void update(){
    //update all the overlap checks
    checkCollisions();
    updateHaptics();
  }
  //-----------------------------------------------------------------------
  public void update(PCInterfaceUnit user0, PCInterfaceUnit user1){
    //update by reading the IO units directly
    //Player0.move(Player0.playerPosition + user0.getUserChange() );
    Player0.shift(user0.getUserChange() );
    Player1.shift(user1.getUserChange() );
    checkCollisions();
    updateHaptics();
    //Now copy haptic responses across
    user0.haptic0Value = Player0.hapticStrength0;
    user0.haptic1Value = Player0.hapticStrength1;
    user1.haptic0Value = Player1.hapticStrength0;
    user1.haptic1Value = Player1.hapticStrength1;
    //copy any LED settings as well...?
    Player0.LEDBrightness = user0.LEDBrightness;
    Player1.LEDBrightness = user1.LEDBrightness;
    
    Player0.buttonState = user0.buttonState;
    Player1.buttonState = user1.buttonState;
  }
  //-----------------------------------------------------------------------
  //move by adding new positions to the existing ones
  public void updateShift(int newPosA, int newPosB){
    Player0.shift(newPosA);
    Player1.shift(newPosB);
    //update all the overlap checks
    checkCollisions();
    updateHaptics();
    //Player0SelfOverlap = Player0.isTouchingStatic(worldWidth);
    //Player1SelfOverlap = Player1.isTouchingStatic(worldWidth);
  }
  //-----------------------------------------------------------------------
  public int wrap(int inPosition) {
    //wrap around the world limit
    int ps = inPosition;
    if(ps < 0) return (worldWidth+ps)% worldWidth;
    return inPosition % worldWidth;
  }
  //-----------------------------------------------------------------------

  public void checkCollisions(){
    Player0SelfOverlap = Player0.isTouchingStatic();
    Player1SelfOverlap = Player1.isTouchingStatic();
    //check to see if the users are touching each other or each others lure
    //Do this first for when they are not wrapping around
    bothUsersTouching = checkUserCrossing(Player0, Player1); //works
    Player0OverlapLure1 = checkLureCrossing(Player0, Player1); //see if A crosses B's lure
    Player1OverlapLure0 = checkLureCrossing(Player1, Player0); //see if B crosses A's lure
    
    if(Player0SelfOverlap||bothUsersTouching||Player0OverlapLure1) Player0Contact = true;
    else Player0Contact = false;
    if(Player1SelfOverlap||bothUsersTouching||Player1OverlapLure0) Player1Contact = true;
    else Player1Contact = false;
  }
  
  public void updateHaptics(){
    if(Player0Contact == true) Player0.setHapticValues(hapticContactValue);
    else                       Player0.setHapticValues(hapticNoContactValue);
    
    if(Player1Contact == true) Player1.setHapticValues(hapticContactValue);
    else                       Player1.setHapticValues(hapticNoContactValue);
    
  }
  //-----------------------------------------------------------------------
  
  public boolean checkUserCrossing(PCPlayer Player0, PCPlayer user2){
    if(Player0.getUserEnd() > Player0.getUserStart() && user2.getUserEnd() > user2.getUserStart()){
      //Neither user is wrapping//println("No Wrap");
      if(Player0.getUserEnd() < user2.getUserStart() || Player0.getUserStart() > user2.getUserEnd()) return false;
      if(user2.getUserEnd() < Player0.getUserStart() || user2.getUserStart() > Player0.getUserEnd()) return false;
    }
    if(Player0.getUserEnd() < Player0.getUserStart() && user2.getUserEnd() > user2.getUserStart()){
      //Player0 is wrapping//println("Player0 Wrap");
      if(user2.getUserStart() > Player0.getUserEnd() && user2.getUserEnd() < Player0.getUserStart()) return false;
    }
    if(user2.getUserEnd() < user2.getUserStart() && Player0.getUserEnd() > Player0.getUserStart()){
      //User2 is wrapping//println("User2 Wrap");
      if(Player0.getUserStart() > user2.getUserEnd() && Player0.getUserEnd() < user2.getUserStart()) return false;
    }
    //if(Player0.getUserEnd() < Player0.getUserStart() && user2.getUserEnd() < user2.getUserStart()) return true; //Both are wrapping - then they are both overlapping as well
    return true;
  }
  //-----------------------------------------------------------------------
  
  public boolean checkLureCrossing(PCPlayer Player0, PCPlayer user2){
    //Returns true if Player0 overlaps user 2's lure
    if(Player0.getUserEnd() > Player0.getUserStart() && user2.getLureEnd() > user2.getLureStart()){
      //Neither the user or the lure are wrapping around
      if(Player0.getUserEnd() < user2.getLureStart() || Player0.getUserStart() > user2.getLureEnd()) return false;
    }
    if(Player0.getUserEnd() < Player0.getUserStart() && user2.getLureEnd() > user2.getLureStart()){
      //User wraps but not lure
      if(Player0.getUserEnd() < user2.getLureStart() && Player0.getUserStart() > user2.getLureEnd()) return false;
    }
    if(Player0.getUserEnd() > Player0.getUserStart() && user2.getLureEnd() < user2.getLureStart()){
      //Lure wraps but not user
      if(Player0.getUserEnd() < user2.getLureStart() && Player0.getUserStart() > user2.getLureEnd()) return false;
    }
    //if(Player0.getUserEnd() < Player0.getUserStart() && user2.getLureEnd() < user2.getLureStart()) return true;
    return true;
  }

}
//class for handling communication between the computer and a single interface unit

//A global serial event handler
public void serialEvent(Serial port) {
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


public boolean NOUnitsConnected(){
  //return true if there are no connected units
  for(int n = 0; n < numberOfUnits; n++){
    if(User[n].portConnected == true) return false;
  }
  return true;
}
public void LEDsOn(){
  for(int n = 0; n < numberOfUnits; n++){
    User[n].sendLEDOn();
  }
}
public void LEDsOff(){
  for(int n = 0; n < numberOfUnits; n++){
    User[n].sendLEDOff();
  }
}
public void beepsOn(){
  for(int n = 0; n < numberOfUnits; n++){
    User[n].sendBeepOn();
  }
}
public void beepsOff(){
  for(int n = 0; n < numberOfUnits; n++){
    User[n].sendBeepOff();
  }
}
public void ledBeepOn(){
  for(int n = 0; n < numberOfUnits; n++){
    User[n].sendLEDOn();
    User[n].sendBeepOn();
  }
}
public void ledBeepOff(){
  for(int n = 0; n < numberOfUnits; n++){
    User[n].sendLEDOff();
    User[n].sendBeepOff();
  }
}

public void setHaptics(){
  //set all haptics
  //use trial mappings to get user haptics and send to correct unit
  //Trial.getUsersGame(0);
  //int userNo = 0;
  for(int n = 0; n < numberOfUnits; n++){
    //this assumes that the environment and users were updated by passing user objects to the appropriate game
    User[n].updateHaptics();
  }
}

public void sendStartRound(String startMessage){
  //Send to all units at once
  for(int n = 0; n < numberOfUnits; n++){
    User[n].sendStartRound(startMessage);
  }
}


public void sendEndRoundMessage(String msg){
  for(int n = 0; n < numberOfUnits; n++){
    User[n].setHapticsMessage(0, 0, msg);
  }
}
//=====================================================================================================================================================================

public void sendHaltRoundAll(boolean aborted){
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
  public void connectPort(PApplet pa){
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
  public void disconnectPort(){
    serialPort.stop();
    serialPort = null;
    portConnected = false;
    println("Disconnected "+String.valueOf(unitNumber));
  }
  //---------------------------------------------------------------------------
  public void checkPort(){
    //check the serial port for new data 
    if(serialPort == null || portConnected == false) return;
    
    //Collect any data
  }
  //---------------------------------------------------------------------------
  public void serialEventHandler(){
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
  public void parseData(String parseThis){
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

  public void resetVariables(){
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
  public void setReplyState_OK(){ 
      newData = true;
      waitingForReply = false;
  } 
  //---------------------------------------------------------------------------
  public void resetTimeout(){ 
      newData = false;
      waitingForReply = false;
      timeouts++;
  }
  //---------------------------------------------------------------------------
  public void sendData(String data){
    if(portConnected == false || serialPort == null) return;
    serialPort.write(data);
    //println("Sent unit "+ String.valueOf(unitNumber) + " Data: " + data);
    if(printDebugMessages) println("Sent "+String.valueOf(unitNumber) +data);
  }
  //---------------------------------------------------------------------------
  public void sendStartRound(String lcdMessage){
    String message = START_ROUND_CMD + lcdMessage + String.valueOf(NEWLINE);
    sendData(message);
    waitingForReply = true;
  }
  //---------------------------------------------------------------------------
  public void sendEndRound(String lcdMessage){
    String message = END_ROUND_CMD + lcdMessage + String.valueOf(NEWLINE);
    sendData(message);
    waitingForReply = true;
  }
  //---------------------------------------------------------------------------
  public void updateHaptics(){
    String message = HAPTICS_CMD + String.valueOf(haptic0Value) + ' ' + String.valueOf(haptic1Value) + String.valueOf(NEWLINE);
    sendData(message);
  }
  
  public void updateHapticsMessage(String lcdMessage){
    String message = HAPTICS_CMD + String.valueOf(haptic0Value) + ' ' + String.valueOf(haptic1Value) + ' ' + lcdMessage + String.valueOf(NEWLINE);
    sendData(message);
  }
  
  public void setHaptics(int h0, int h1){
    haptic0Value = h0;
    haptic1Value = h1;
    String message = HAPTICS_CMD + String.valueOf(haptic0Value) + ' ' + String.valueOf(haptic1Value) + String.valueOf(NEWLINE);
    sendData(message);
  }
  //---------------------------------------------------------------------------
  public void setHapticsMessage(int h0, int h1, String lcdMessage){
    haptic0Value = h0;
    haptic1Value = h1;
    String message = HAPTICS_MESSAGE_CMD + String.valueOf(h0) + ' ' + String.valueOf(h1) + ' ' + lcdMessage + String.valueOf(NEWLINE);
    sendData(message);
  }
  //---------------------------------------------------------------------------
  public void getState(){
    sendData(GET_STATE);
    waitingForReply = true;
  }
  //---------------------------------------------------------------------------
  public void getStateMessage(String lcdMessage){
    String message = GET_STATE_MESSAGE_CMD + lcdMessage + String.valueOf(NEWLINE);
    sendData(message);
    waitingForReply = true;
  }
  //---------------------------------------------------------------------------
  public void sendBeepOn(){ sendData(BEEP_ON);}
  //---------------------------------------------------------------------------
  public void sendBeepOff(){sendData(BEEP_OFF);}
  //---------------------------------------------------------------------------
  public void sendLEDOn(){sendData(LED_ON);}
  //---------------------------------------------------------------------------
  public void sendLEDOff(){sendData(LED_OFF);}
  //---------------------------------------------------------------------------
  public void queryDevice(){sendData(QUERY);}
  
  public int getUserChange(){
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
  public void resetUserChange(){
    xChange = 0;
    yChange = 0;
  }
}



public void runRound(){
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
public void trialMarkON(){
  if(Trial.useBeepMark == true && Trial.useLEDMark == true) ledBeepOn();
  else if(Trial.useLEDMark == true) LEDsOn();
  else if(Trial.useBeepMark == true) beepsOn();
}
//=====================================================================================================================================================================
public void trialMarkOFF(){
  if(Trial.useBeepMark == true && Trial.useLEDMark == true) ledBeepOff();
  else if(Trial.useLEDMark == true) LEDsOff();
  else if(Trial.useBeepMark == true) beepsOff();
}

//=====================================================================================================================================================================

public void updateIntervals(){
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

public void initEnvironments(int staticPos0, int staticPos1){
  //randomise positions
  for(int n = 0; n < numberOfEnvironments; n++){
    //Setup random user positions:
    Env[n].Player0.playerPosition = PApplet.parseInt(random(0,environmentWidth));
    Env[n].Player1.playerPosition = PApplet.parseInt(random(0,environmentWidth));
    Env[n].Player0.move(Env[n].Player0.playerPosition);
    Env[n].Player1.move(Env[n].Player1.playerPosition);
    //reset the static object positions
    
    Env[n].Player0.setStaticParameters(staticObjectWidth, staticPos0);
    Env[n].Player1.setStaticParameters(staticObjectWidth, staticPos1);
  }
}
//=====================================================================================================================================================================

public boolean waitForReply(int timeout){
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

public boolean checkReplyStatusAll(){
  //return true if any unit is waiting for a reply
  int replies = 0;
 for(int n = 0; n < numberOfUnits; n++){
   if( User[n].waitingForReply == true  && User[n].portConnected == true) replies++;
 }
 if(replies > 0) return true;
 return false;
}

//=====================================================================================================================================================================

public void updateManual(){
 //do nothing 
 drawGraphics();
  stroke(0);
  fill(0);
  practiceIterations++;
}
//=====================================================================================================================================================================

public void updateFreePractice(){
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
public void updateMouseMode(){
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
public void resetEnvironments(){
  for(int n = 0; n < numberOfEnvironments; n++){
    Env[n].Player0.setUserParameters(usersWidth, luresWidth, gapToLure, staticObjectWidth, player0StaticObjectPosition);
    Env[n].Player1.setUserParameters(usersWidth, luresWidth, gapToLure, staticObjectWidth, player1StaticObjectPosition);
  }
}
public void randomisePlayerPositions(){
    //Setup random user positions:
  for(int n = 0; n < numberOfEnvironments; n++){
    Env[n].Player0.playerPosition = PApplet.parseInt(random(0,environmentWidth));
    Env[n].Player1.playerPosition = PApplet.parseInt(random(0,environmentWidth));
    Env[n].Player0.move(Env[n].Player0.playerPosition);
    Env[n].Player1.move(Env[n].Player1.playerPosition);
  }
}
public void randomiseStaticPositions(){
    //Setup random user positions:
  for(int n = 0; n < numberOfEnvironments; n++){
    Env[n].Player0.setStaticParameters(Env[n].Player0.staticWidth , PApplet.parseInt(random(environmentWidth))); //width, position
    Env[n].Player1.setStaticParameters(Env[n].Player1.staticWidth , PApplet.parseInt(random(environmentWidth)));
  }
}

public void randomiseDimensions(){//Setup random user sizes:
  for(int n = 0; n < numberOfEnvironments; n++){
    Env[n].Player0.setUserParameters(PApplet.parseInt(random(2,16)), PApplet.parseInt(random(2,16)), PApplet.parseInt(random(10, 400)), PApplet.parseInt(random(2,16)), Env[n].Player0.staticPos);
    Env[n].Player1.setUserParameters(PApplet.parseInt(random(2,16)), PApplet.parseInt(random(2,16)), PApplet.parseInt(random(10, 400)), PApplet.parseInt(random(2,16)), Env[n].Player0.staticPos);
    
    Env[n].Player0.move(PApplet.parseInt(random(environmentWidth)));
    Env[n].Player1.move(PApplet.parseInt(random(environmentWidth)));
    Env[n].Player0.setStaticParameters(PApplet.parseInt(random(2,16)), Env[n].Player0.staticPos); //width, position
    Env[n].Player1.setStaticParameters(PApplet.parseInt(random(2,16)), Env[n].Player1.staticPos);
  }
}


long roundLengthMillis = roundLengthSeconds*1000;
//timing in seconds for the start beep/flash
//The middle beep/flash 
//the end beep flash

//ALL ARE IN MILLISECONDS

  public static final int MANUAL = 0;
  public static final int FREE_PRACTICE = 1;
  public static final int TRIAL_ACTIVE = 2;

//int roundState = 0;
String diadMap0 = "0 Vs 1 and 2 Vs 3";
String diadMap1 = "0 Vs 2 and 1 Vs 3";
String diadMap2 = "0 Vs 3 and 1 Vs 2";
//List playerMapListText = Arrays.asList("0 vs 1 and 2 vs 3", "0 vs 2 and 1 vs 3", "0 vs 3 and 1 vs 2");
List playerMapListText = Arrays.asList(diadMap0, diadMap1, diadMap2);
//List<String> myString = new ArrayList<String>();
//myString.add("Strings"); add(diadMap0);

public void startTrial(){
  //start logging data
  //update the trial number
  println("Starting Trial");
  Trial.startTrial();
  cp5.get(Textlabel.class,"TrialLabel").setText(String.valueOf(Trial.trialNumber));
  //Trial.roundNumber = 0;
  cp5.get(Textlabel.class,"RoundLabel").setText(String.valueOf(Trial.roundNumber));
  //Trial.trialMode = TRIAL_ACTIVE;
  startLogs();
  
  replyTimeouts = 0;
  cp5.get(Textlabel.class,"trialStatusLabel").setText("Trial Running");
}

public void stopTrial(){
  
  println("Stopping Trial");
  //stop logging data
  stopLogs();
  Trial.stopTrial();
  cp5.get(Textlabel.class,"trialStatusLabel").setText("Practice Mode");
} 


public void startRound(){
  //write a new file header for this round
  if(Trial.roundNumber > 0) writeFileHeader();
  practiceIterations = 0;
  
  println("Running Round ...");
  Trial.startRound();
  //Trial.roundActive = true;
  cp5.get(Textlabel.class,"RoundLabel").setText(String.valueOf(Trial.roundNumber));
  cp5.get(Textlabel.class,"roundStatusLabel").setText("Round Active");
}

public void endRound(){
  println("Ending Round ...");
  cp5.get(Textlabel.class,"roundStatusLabel").setText("Round Finished");
  Trial.stopRound();
  //Trial.roundActive = false; //just for debugging
  //Trial.roundNumber++;
}
//=====================================================================================================================================================================
public boolean checkExitKey(){
  if(keyPressed){
    sendHaltRoundAll(true);
    return true;
  }
  return false;
}


class TrialController 
{
  public
  int trialMode = FREE_PRACTICE;
  int roundNumber = 0;
  int trialNumber = 0;
  //boolean trialActive = false;
  boolean roundActive = false;
  
  long startMessageTime = 2500;
  long endMessageTime = 5000;
  long startindicatorLength = 1000;
  boolean startIndicatorOn = false;

  boolean middleIndicatorLEDEnabled = false;
  boolean middleIndicatorBeepEnabled = false;
  long middleIndicatorTime = 5000;
  long middleIndicatorLength = 500;
  boolean middleIndicatorOn = false;
  boolean middleIndicatorDone = false;
  long endIndicatorLength = 1000;

  boolean useBeepMark = false; //true of start and end points are marked with a beep
  boolean useLEDMark = true;//true of start and end points are marked with a flash
  int numberOfPlayers = 2;
  int numberOfGames = 1;
  int playerPairs[][];
  int playerMapping = 0;
  //constructer
  TrialController(int environments){
    numberOfGames = environments;
    numberOfPlayers = numberOfPlayers*2;
    playerPairs = new int[numberOfGames][2];
  }
  
  public void startTrial(){
    trialMode = TRIAL_ACTIVE;
    roundNumber = 0;
  }
  
  public void stopTrial(){
    trialNumber++;
    roundNumber = 0;
    trialMode = FREE_PRACTICE;
  }
  
  public void startRound(){
    //trialMode = TRIAL_ACTIVE;
    roundActive = true;
  }
  
  public void stopRound(){
    //trialMode = FREE_PRACTICE;
    roundActive = false;
    roundNumber++;
  }
  
  
  public void initialiseMapping(){
    //map each player to their neighbor
    for(int n = 0; n < numberOfGames; n++){
      playerPairs[n][0] = n;
      playerPairs[n][1] = numberOfGames+n;
    }
  }
  public int getUsersGame(int userNo){
    //return the game that the given user is in
    for(int n = 0; n < numberOfGames; n++){
      if(playerPairs[n][0] == userNo || playerPairs[n][1] == userNo) return n;
    }
    return 0;
  }
  public int getPlayerSlot(int userNo){
    //return the player slot (A or B) that the player is assigned to within their game
    //Returns 0 for user A and 1 for user B
    int game = getUsersGame(userNo);
    if(playerPairs[game][0] == userNo) return 0;
    else return 1;
  }
  public int getUsersOpponent(int userNo){
    //return the game that the given user is in
    int game = getUsersGame(userNo);
    if(playerPairs[game][0] == userNo) return playerPairs[game][1];
    else return playerPairs[game][0];
  }
  
  public int getPlayer0User(int environmentNo){
    //get the user assigned to player0 for this environment
      return playerPairs[environmentNo][0];
  }
  public int getPlayer1User(int environmentNo){
    //get the user assigned to player0 for this environment
      return playerPairs[environmentNo][1];
  }
  
  /*
  2D array with x as the number of games
  and Y as the number of users
  for each game, each player in that game is given a number that relates to their user
  so this
  playerPairs[GAME ZERO][0] = 1; means that for game 0 player 0 is user 1
  and
  playerPairs[GAME ZERO][1] = 0; means that for game 0 player 1 user 0
  */
  
  public void setMapping(int mapNo){
    //fudge this for now
    playerMapping = mapNo;
    if(playerMapping > 2) playerMapping = 0;
    if(playerMapping < 0) playerMapping = 0;
    switch(numberOfGames){
      case 1:
        playerPairs[0][0] = 0; //player 0 is user o
        playerPairs[0][1] = 1;
        break;
      case 2:
              //GAME - Player
        if(playerMapping == 0){
          playerPairs[0][0] = 0; //Game 0, Player 0 is User 0
          playerPairs[0][1] = 1; //Game 0, Player 0 is User 1
          playerPairs[1][0] = 2; //Game 1, Player 1 is User 2
          playerPairs[1][1] = 3; //Game 1, Player 1 is User 3
        }
        if(playerMapping == 1){
          playerPairs[0][0] = 0; //Game 0, Player 0 is User 0
          playerPairs[0][1] = 2; //Game 0, Player 1 is User 2
          playerPairs[1][0] = 1; //Game 1, Player 0 is User 1
          playerPairs[1][1] = 3; //Game 1, Player 1 is User 3
        }
        if(playerMapping == 2){
          playerPairs[0][0] = 0; //Game 0, Player 0 is User 0
          playerPairs[0][1] = 3; //Game 0, Player 1 is User 3
          playerPairs[1][0] = 1; //Game 1, Player 0 is User 1
          playerPairs[1][1] = 2; //Game 1, Player 1 is User 2
        }
        break;
    }
  }

}


public void updatePlayerMappings(){
  
  
}

public void updateTimingData(){
  //copy env params to system from data  entry boxes
  String textValue = cp5.get(Textfield.class, "EnvironmentWidth").getText();
  environmentWidth = Integer.parseInt(textValue);
  environmentStart = (width/2) - (environmentWidth/2); //The distance from the screen edge that tne environment starts
  Env[0].setWorldWidth(environmentWidth);

  String textValue2 = cp5.get(Textfield.class, "RoundLength").getText();
  roundLengthMillis = (long)Integer.parseInt(textValue2);
  roundLengthSeconds = (int)roundLengthMillis / 1000;
  
  String textValue3 = cp5.get(Textfield.class, "StartBeepLength").getText();
  Trial.startindicatorLength = Integer.parseInt(textValue3);

  String textValue4 = cp5.get(Textfield.class, "MiddleBeepTime").getText();
  Trial.middleIndicatorTime = Integer.parseInt(textValue4);
  
  String textValue5 = cp5.get(Textfield.class, "MiddleBeepLength").getText();
  Trial.middleIndicatorLength = Integer.parseInt(textValue5);
  
  String textValue6 = cp5.get(Textfield.class, "EndBeepLength").getText();
  Trial.endIndicatorLength = Integer.parseInt(textValue6);
  
}
public int boolToInt(boolean b) {
    return Boolean.compare(b, false);
}
public void writeFileHeader(){
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


public void logTrialData(){
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


public void startLogs(){
  //startLogA();
  //startLogB();
  startLogCombined();
}

public void stopLogs(){
  //stopLogA();
  //stopLogB();
  stopLogCombined();
}
public void startLogCombined(){
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

public void stopLogCombined(){
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

public void startLogA(){
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


public void stopLogA(){
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

public void startLogB(){
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

public void stopLogB(){
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

public void logDataCombined(String dataLine){
   if(logCombinedOpen == false) return;
   LogFileCombined.append(dataLine);
}

public void logDataA(String dataLine){
   if(logAOpen == false) return;
   LogFileA.append(dataLine);
}

public void logDataB(String dataLine){
   if(logBOpen == false) return;
   LogFileB.append(dataLine);
}
//configuration options for the trial

public int setupMessages(int xp, int yp, int gap){
  int xps = xp;
  int yps = yp;
  
   PFont textFont = createFont("arial", 20);
   cp5.addTextfield("StartRoundTextbox")
    .setPosition(xps,  yps)
      .setSize(500, 40)
        .setFont(textFont)
          .setFocus(true)
            .setColor(color(255))
              .setText(startTrialMessage)
              .setCaptionLabel("Start of round message")
              
             .setColorLabel(0)
                .moveTo("Configuration");
              ;
  yps += gap;
  cp5.addTextfield("EndRoundTextbox")
    .setPosition(xps,  yps)
      .setSize(500, 40)
        .setFont(textFont)
          .setFocus(true)
            .setColor(color(255))
              .setText(endTrialMessage)
              .setCaptionLabel("End of round message")
              
             .setColorLabel(0)
                .moveTo("Configuration");
              ;
  return yps+gap;
  //cp5.get(Textfield.class, "StartTrialTextbox").getText()
}

public void setupTrackerCon(int xp, int yp){
  cp5.addTextlabel("TrackerballModeLabel")
      .setText("Trackerball Mode")
      .setPosition(xp,yp)
      .setColorValue(100)
      .setFont(createFont("Ariel",12))
      .moveTo("Configuration");
    ;
  cp5.addScrollableList("trackerballMapping")
   .setPosition(xp, yp+20)
   .setSize(listBoxWidth, listBoxHeight)
   .setBarHeight(20)
   .setItemHeight(20)
   .setColorLabel(255)
   //.addItems(portList)
   .setLabel("Trackerball Mode")
   .addItem("X Only", 0)
   .addItem("Y Only", 1)
   .addItem("X-Y/2", 2)
   .addItem("X+Y/2 ", 3)
   .setType(ScrollableList.DROPDOWN ) // currently supported DROPDOWN and LIST
   .close()
   .moveTo("Configuration");
   ;
  cp5.getController( "trackerballMapping" ).setValue(0);
}
public void setupMapCon(int xp, int yp){
  cp5.addTextlabel("GameMapLabel")
      .setText("Diad Mapping")
      .setPosition(xp,yp)
      .setColorValue(100)
      .setFont(createFont("Ariel",12))
      .moveTo("Configuration");
    ;
  cp5.addScrollableList("gameMapping")
   .setPosition(xp, yp+20)
   .setSize(listBoxWidth, listBoxHeight)
   .setBarHeight(20)
   .setItemHeight(20)
   .setColorLabel(255)
   //.addItems(portList)
   .setLabel("Player Mapping Mode")
   .addItems(playerMapListText)
   .setType(ScrollableList.DROPDOWN ) // currently supported DROPDOWN and LIST
   .close()
   .moveTo("Configuration");
   ;

  
  cp5.getController( "gameMapping" ).setValue(0);
}
  
  
  
public int setupSetButton(int xp, int yp, int gap){
  int xps = xp;
  int yps = yp;
  int buttonW = 100;
  int buttonH = 30;
  
  cp5.addButton("SetEnvParam")
   .setBroadcast(false)
   .setValue(0)
   .setPosition(xps, yps)
   .setSize(buttonW, buttonH)
   .setFont(createFont("Arial",buttonFontSize))
   .setLabel("SET")
   .setColorLabel(255)
   .setBroadcast(true)
   .moveTo("Configuration");
   ;
    return yps+gap;
}
public int setupParameters(int xp, int yp, int gap){
  int xps = xp;
  int yps = yp;
  cp5.addTextfield("EnvironmentWidth")
   .setPosition(xps, yps)
   .setSize(100,30)
   .setFont(createFont("Arial",buttonFontSize))
   .setLabel("Environment Width")
   .setColorLabel(0)
   //.setFocus(true)
   .setText(String.valueOf(environmentWidth))
   .setAutoClear(false)
   .moveTo("Configuration");
   ;

   return yps+gap;
}


public int setupTiming(int xp, int yp, int gap){
  int xps = xp;
  int yps = yp;
  cp5.addTextfield("RoundLength")
   .setPosition(xps, yps)
   .setSize(100,30)
   .setFont(createFont("Arial",buttonFontSize))
   .setColorLabel(0)
   .setLabel("Round Length (Milliseconds)")
   .setText(String.valueOf(roundLengthMillis))
   .setAutoClear(false)
   .setFocus(true)
   .moveTo("Configuration");
   ;
   yps+=gap;
  
  cp5.addTextfield("StartBeepLength")
   .setPosition(xps,yps)
   .setSize(100,30)
   .setFont(createFont("Arial",buttonFontSize))
   .setColorLabel(0)
   .setLabel("Start Beep Length (Milliseconds)")
   .setText(String.valueOf(Trial.startindicatorLength))
   .setAutoClear(false)
   .setFocus(true)
   .moveTo("Configuration");
   ;
   
  yps+=gap;
  
  cp5.addTextfield("MiddleBeepTime")
   .setPosition(xps,yps)
   .setSize(100,30)
   .setFont(createFont("Arial",buttonFontSize))
   .setColorLabel(0)
   .setLabel("Mid Beep Time (Milliseconds)")
   .setText(String.valueOf(Trial.middleIndicatorTime))
   .setAutoClear(false)
   .setFocus(true)
   .moveTo("Configuration");
   ;
   
  yps+=gap;
  
  cp5.addTextfield("MiddleBeepLength")
   .setPosition(xps, yps)
   .setSize(100,30)
   .setFont(createFont("Arial",buttonFontSize))
   .setColorLabel(0)
   .setLabel("Mid Beep Length (Milliseconds)")
   .setText(String.valueOf(Trial.middleIndicatorLength) )
   .setAutoClear(false)
   .setFocus(true)
   .moveTo("Configuration");
   ;
   
   yps+=gap;
   cp5.addTextfield("EndBeepLength")
   .setPosition(xps,yps)
   .setSize(100,30)
   .setFont(createFont("Arial",buttonFontSize))
   .setColorLabel(0)
   .setLabel("End Beep length (Milliseconds)")
   .setText(String.valueOf(Trial.endIndicatorLength))
   .setAutoClear(false)
   .setFocus(true)
   .moveTo("Configuration");
   ;
   
  yps+=gap;
   return yps+gap;
}

StringList manualLEDID;
StringList manualBeeperID;
StringList manualVibID;


public void setupManual(int px, int py, int gapX, int gapY, int quantity){
  int posX = px;
  int posY = py;
  
  manualLEDID = new StringList();
  manualBeeperID = new StringList();
  manualVibID = new StringList();
  for(int n = 0; n < quantity; n++){
    String unitNo = String.valueOf(n);
   //setup buttons to start and stop beeps and led's
  cp5.addTextlabel("ManualUnitlabel" + unitNo)
    .setText("Unit " + unitNo)
    .setPosition(posX,posY)
    //.getCaptionLabel()
    .setColorValue(0)
    .setFont(createFont("Arial",20))
    //.setFont(createFont("Georgia",20))
    .moveTo("Manual");
    ;
  posY += gapY;
  
  manualLEDID.append("LED" + unitNo);
  cp5.addToggle("LED" + unitNo)
     .setBroadcast(false)
     .setPosition(posX, posY)
     .setSize(50,30)
     .setValue(false)
     .setMode(ControlP5.DEFAULT)
     .setColorLabel(0)
     .setLabel("LED") 
     .setLabelVisible(true)  
     .setBroadcast(true)
     .moveTo("Manual");
     ;
  posY += gapY;
  
  manualBeeperID.append("Beep" + unitNo);
  cp5.addToggle("Beep" + unitNo)
     .setBroadcast(false)
     .setPosition(posX, posY)
     .setSize(50,30)
     .setValue(false)
     .setMode(ControlP5.DEFAULT)
     .setColorLabel(0)
     .setLabel("Beep") 
     .setLabelVisible(true)  
     .setBroadcast(true)
     .moveTo("Manual");
     ;
     
  posY += gapY;
  manualVibID.append("Vib" + unitNo);
  cp5.addToggle("Vib" + unitNo)
     .setBroadcast(false)
     .setPosition(posX, posY)
     .setSize(50,30)
     .setValue(false)
     .setMode(ControlP5.DEFAULT)
     .setColorLabel(0)
     .setLabel("Vib") 
     .setLabelVisible(true)  
     .setBroadcast(true)
     .moveTo("Manual");
     ;
    posX += gapX;
    posY = py;
  }
   //cp5.getController("Unitlabel" + unitNo).moveTo("Manual");
   //cp5.getController("LED" + unitNo).moveTo("Manual");
   //cp5.getController("Beep" + unitNo).moveTo("Manual");
}


/*void LED1(boolean theFlag){
  if(theFlag == true) sendLedOn(1);
  else                sendLedOff(1);
}
void Beep1(boolean theFlag){
  if(theFlag == true) sendBeepOn(1);
  else                sendBeepOff(1);
}*/
/*
void setupManual2(int px, int py, int gap){
  int posX = px;
  int posY = py;
 //setup buttons to start and stop beeps and led's
 cp5.addToggle("LED2")
   .setBroadcast(false)
   .setPosition(posX, posY)
   .setSize(50,30)
   .setValue(false)
   .setMode(ControlP5.DEFAULT)
   .setColorLabel(0)
   .setLabel("LED") 
   .setLabelVisible(true)  
   .setBroadcast(true)
   .moveTo("Manual");
   ;
  posY += gap;
  cp5.addToggle("Beep2")
   .setBroadcast(false)
   .setPosition(posX,posY)
   .setSize(50,30)
   .setValue(false)
   .setMode(ControlP5.DEFAULT)
   .setColorLabel(0)
   .setLabel("Beep") 
   .setLabelVisible(true)  
   .setBroadcast(true)
   .moveTo("Manual");
   ;

   //cp5.getController("LED2").moveTo("Manual");
   //cp5.getController("Beep2").moveTo("Manual");
}*/

//int startAX = 10;
//int startBX = 10; //initialised with window width in startup
//int startAY = 10;
//int startBY = 10;

StringList serialConnectID;
StringList serialListID;

public void setupSerialDisplays(int px, int py, int gapX, int gapY, int quantity){
  if(quantity < 1) return;
  serialConnectID = new StringList();
  serialListID = new StringList();
  int posX = px;
  int posY = py;
  for(int n = 0; n < quantity; n++){
    String item = String.valueOf(n);
    cp5.addTextlabel("SerialControlHeading" + item)
      .setText("Unit " + item)
      .setPosition(posX,posY)
      //.getCaptionLabel()
      .setColorValue(0)
      .setFont(createFont("Arial",20))
      .moveTo("Serial");
    ;
    posY += gapY;
    
    serialConnectID.append("ConnectButton" + item);
    cp5.addToggle("ConnectButton" + item)
     .setBroadcast(false)
     .setPosition(posX, posY)
     .setSize(100,30)
     .setValue(false)
     .setMode(ControlP5.DEFAULT)
     .setColorLabel(0)
     .setLabel("Connect") 
     .setLabelVisible(true)  
     .setBroadcast(true)
     .moveTo("Serial");
     ;
    posY += gapY;
    cp5.addTextlabel("SerialText" + item)
      .setText("SerialData " + item)
      .setPosition(posX,posY)
      .setColorValue(100)
      .setFont(createFont("Ariel",12))
      .moveTo("Serial");
    ;
    posY += gapY;
    serialListID.append("COMMPortSelection" + item);
    cp5.addScrollableList("COMMPortSelection" + item)
     .setPosition(posX, posY)
     .setSize(listBoxWidth, listBoxHeight)
     .setBarHeight(20)
     .setItemHeight(20)
     .setColorLabel(255)
     .addItems(portList)
     .setType(ScrollableList.DROPDOWN ) // currently supported DROPDOWN and LIST
     .close()
     .moveTo("Serial");
     ;
     posX += gapX;
     posY = py;
  }
}

public void setupConnectButtons(int px, int py, int gap){
  int posX = px;
  int posY = py;
  cp5.addToggle("ConnectAButton")
   .setBroadcast(false)
   .setPosition(posX, posY)
   .setSize(100,30)
   .setValue(false)
   .setMode(ControlP5.DEFAULT)
   .setColorLabel(0)
   .setLabel("Connect") 
   .setLabelVisible(true)  
   .setBroadcast(true)
   .moveTo("Serial");
   ;
  posX += gap;
  cp5.addToggle("ConnectBButton")
   .setBroadcast(false)
   .setPosition(posX, posY)
   .setSize(100,30)
   .setValue(false)
   .setMode(ControlP5.DEFAULT)
   .setColorLabel(0)
   .setLabel("Connect") 
   .setLabelVisible(true)  
   .setBroadcast(true)
   .moveTo("Serial");
   ;
   
  // cp5.getController("ConnectAButton").moveTo("Serial");
   //cp5.getController("ConnectBButton").moveTo("Serial");
}


public void setupListBoxes(int px, int py, int gap){
  //int startPos = startAY+50;
  
  int posX = px;
  int posY = py;
  cp5.addScrollableList("COMMPortA")
   .setPosition(posX, posY)
   .setSize(listBoxWidth, listBoxHeight)
   .setBarHeight(20)
   .setItemHeight(20)
   .setColorLabel(255)
   .addItems(portList)
   .setType(ScrollableList.DROPDOWN ) // currently supported DROPDOWN and LIST
   .close()
   .moveTo("Serial");
   ;
  posX += gap;   
  cp5.addScrollableList("COMMPortB")
   .setPosition(posX, posY)
   .setSize(listBoxWidth, listBoxHeight)
   .setBarHeight(20)
   .setItemHeight(20)
   .setColorLabel(255)
   .addItems(portList)
   .setType(ScrollableList.DROPDOWN ) // currently supported DROPDOWN and LIST
   .close()
   .moveTo("Serial");
   ;
   //cp5.getController("COMMPortA").moveTo("Serial");
   //cp5.getController("COMMPortB").moveTo("Serial");
}
/*
void setupSerialDisplay(int px, int py, int gap){
  
  int posX = px;
  int posY = py;
  serialTextA = cp5.addTextlabel("SerialTextA")
    .setText("SerialDataA")
    .setPosition(posX,posY)
    .setColorValue(0xffffff00)
    .setFont(createFont("Georgia",16))
   .moveTo("Serial");
    ;
  posX += gap;
  serialTextB = cp5.addTextlabel("SerialTextB")
    .setText("SerialDataB")
    .setPosition(posX,posY)
    .setColorValue(0xffffff00)
    .setFont(createFont("Georgia",16))
    .moveTo("Serial");
    ;
   //cp5.getController("SerialTextA").moveTo("Serial");
   //cp5.getController("SerialTextB").moveTo("Serial");
}*/
int buttonFontSize = 14;
String controlSubjectIDName = "SubjectNumber_";
public void setupParticipentDataEntry(int xp, int yp, int gap, int quantity){
  int yps = yp;
  
  
  for(int qty = 0; qty < quantity; qty++){
    cp5.addTextfield(controlSubjectIDName + String.valueOf(qty))
      .setPosition(xp, yps)
      .setSize(100, 30)
      .setFont(createFont("Arial",buttonFontSize))
      .setColorLabel(0)
      .setLabel("User " + qty + " Subject ID")
      .setColor(color(0))
      .setColorBackground(color(250))
      ;
    yps += gap;
  }
  /*cp5.addButton("TestButton")
   .setBroadcast(false)
   .setPosition(xp, yps)
   .setSize(30,30)
   .setFont(createFont("Arial",buttonFontSize))
   .setColorLabel(255)
   .setLabel("Test") 
   .setLabelVisible(true)  
   .setBroadcast(true)
   ;*/
}

/*public void TestButton(int theValue){
  for(int n = 0; n < numberOfUnits; n++){
    println("SubjectText = " + getSubjectID(n));
  }
}*/


public String getSubjectID(int unitNumber){
  if(unitNumber > numberOfUnits || unitNumber < 0) return " ";
  String controlName = controlSubjectIDName+String.valueOf(unitNumber);
  return cp5.get(Textfield.class, controlName).getText();
  //controlSubjectIDName
  //return " ";
}

public int setupAnimationButton(int xp, int yp, int gap){
  int xps = xp;
  int yps = yp;
  cp5.addToggle("animation")
   .setBroadcast(false)
   .setPosition(xps, yps)
   .setSize(100,30)
   .setValue(true)
   .setMode(ControlP5.DEFAULT)
   .setFont(createFont("Arial",buttonFontSize))
   .setColorLabel(0)
   .setLabel("Toggle Animation") 
   .setLabelVisible(true)  
   .setBroadcast(true)
   ;
    return yps+gap;
}

public int setupMouseConButton(int xp, int yp, int gap){
  int xps = xp;
  int yps = yp;
  cp5.addToggle("MouseMode")
   .setBroadcast(false)
   .setPosition(xps, yps)
   .setSize(100,30)
   .setValue(false)
   .setMode(ControlP5.DEFAULT)
   .setFont(createFont("Arial",buttonFontSize))
   .setColorLabel(0)
   .setLabel("Enable Mouse") 
   .setLabelVisible(true)  
   .setBroadcast(true)
   ;
    return yps+gap;
}


public int setupRandomButtons(int xp, int yp, int bWidth,int gap){
  int xps = xp;
  int yps = yp;
  cp5.addButton("RandomPos")
   .setBroadcast(false)
   .setPosition(xps, yps)
   .setSize(bWidth,30)
   .setFont(createFont("Arial",buttonFontSize))
   .setColorLabel(255)
   .setLabel("Randomise User Positions") 
   .setLabelVisible(true)  
   .setBroadcast(true)
   ;
   yps+=gap;
  cp5.addButton("RandomStatic")
   .setBroadcast(false)
   .setPosition(xps, yps)
   .setSize(bWidth,30)
   .setFont(createFont("Arial",buttonFontSize))
   .setColorLabel(255)
   .setLabel("Randomise Static Object Position") 
   .setLabelVisible(true)  
   .setBroadcast(true)
   ;
   yps+=gap;
   cp5.addButton("RandomDim")
   .setBroadcast(false)
   .setPosition(xps, yps)
   .setSize(bWidth,30)
   .setFont(createFont("Arial",buttonFontSize))
   .setColorLabel(255)
   .setLabel("Randomise Sizes") 
   .setLabelVisible(true)  
   .setBroadcast(true)
   ;
   yps+=gap;
   cp5.addButton("ResetEnv")
   .setBroadcast(false)
   .setPosition(xps, yps)
   .setSize(bWidth,30)
   .setFont(createFont("Arial",buttonFontSize))
   .setColorLabel(255)
   .setLabel("Reset") 
   .setLabelVisible(true)  
   .setBroadcast(true)
   ;
    return yps+gap;
}
/*
void LED2(boolean theFlag){
  if(theFlag == true) sendLedOn(2);
  else                sendLedOff(2);
}
void Beep2(boolean theFlag){
  if(theFlag == true) sendBeepOn(2);
  else                sendBeepOff(2);
}*/

public void setupLog(){
  int cCentre = width/2;
  int bWidth = 100;
  
  /*cp5.addToggle("LogDataButton")
     .setBroadcast(false)
     .setPosition(cCentre-(bWidth/2), startAY)
     .setSize(bWidth,30)
     .setValue(false)
     .setMode(ControlP5.DEFAULT)
     .setLabel("Start Log") 
     .setLabelVisible(true)  
     .setBroadcast(true)
     ;*/
     
}

public void setupDiadControls(int posX, int posY, int yGap){
  int xp = posX, yp = posY;
  int buttonW = 200;
  int buttonH = 50;
  int fSize = 15;
  cp5.addButton("ShiftDiad")
     .setBroadcast(false)
     .setValue(0)
     .setPosition(xp, yp)
     .setSize(buttonW, buttonH)  
     .setBroadcast(true)
     .setCaptionLabel("Shift Dyads")
     .getCaptionLabel()
     .setFont(largeFont)
     .toUpperCase(false)
     .setSize(fSize)
     ;
  yp+= yGap;
  cp5.addTextlabel("DiadOption")
     .setText(String.valueOf(Trial.trialNumber))
     .setPosition(xp, yp)
     .setColorValue(0)
     .setFont(createFont("Arial",buttonFontSize*2))
     .setText((String)playerMapListText.get(Trial.playerMapping))
     ;
}

public void setupCommandButtons(){
  int buttonW = 300;
  int buttonH = 50;
  int vPos = 35;
  int vSpace = buttonH+5;
  int centreStart = (width/2)-(buttonW/2);
  
  int fSize = 15;
     

  cp5.addButton("ManualMode")
     .setBroadcast(false)
     .setValue(0)
     .setPosition(centreStart, vPos)
     .setSize(buttonW, buttonH)  
     .setBroadcast(true)
     .setCaptionLabel("Manual Mode")
     .getCaptionLabel()
     .setFont(largeFont)
     .toUpperCase(false)
     .setSize(fSize)
     ;
   vPos += vSpace;
  cp5.addButton("FreePractice")
     .setBroadcast(false)
     .setValue(0)
     .setPosition(centreStart, vPos)
     .setSize(buttonW, buttonH)  
     .setBroadcast(true)
     .setCaptionLabel("Free Practice Mode")
     .getCaptionLabel()
     .setFont(largeFont)
     .toUpperCase(false)
     .setSize(fSize)
     ;
     
     vPos += vSpace+20;
  cp5.addButton("StartTrialButton")
     .setBroadcast(false)
     .setValue(0)
     .setPosition(centreStart, vPos)
     .setSize(buttonW, buttonH)  
     .setBroadcast(true)
     .setCaptionLabel("Start Trial")
     .getCaptionLabel()
     .setFont(largeFont)
     .toUpperCase(false)
     .setSize(fSize)
     ;
   vPos += vSpace;
  cp5.addButton("StopTrialButton")
     .setBroadcast(false)
     .setValue(0)
     .setPosition(centreStart, vPos)
     .setSize(buttonW, buttonH)  
     .setBroadcast(true)
     .setCaptionLabel("Stop Trial")
     .getCaptionLabel()
     .setFont(largeFont)
     .toUpperCase(false)
     .setSize(fSize)
     ;
   cp5.addTextlabel("TrialLabel")
      .setText(String.valueOf(Trial.trialNumber))
      .setPosition(centreStart+ buttonW + 10, vPos+5)
      .setColorValue(0)
      .setFont(createFont("Arial",buttonFontSize*2))
      ;
      
      
      
   vPos += vSpace+20;
  cp5.addButton("StartRoundButton")
     .setBroadcast(false)
     .setValue(0)
     .setPosition(centreStart, vPos)
     .setSize(buttonW, buttonH)  
     .setBroadcast(true)
     .setCaptionLabel("Start Round")
     .getCaptionLabel()
     .setFont(largeFont)
     .toUpperCase(false)
     .setSize(fSize)
     ;
   cp5.addTextlabel("RoundLabel")
      .setText(String.valueOf(Trial.roundNumber))
      .setPosition(centreStart+ buttonW + 10, vPos+5)
      .setColorValue(0)
      .setFont(createFont("Arial",buttonFontSize*2))
      ;
   
   
   vPos += vSpace; 
   cp5.addTextlabel("trialStatusLabel")
      .setText("Practice Mode")
      .setPosition(centreStart, vPos+5)
      .setColorValue(0)
      .setFont(createFont("Arial",buttonFontSize*2))
      ;  
   vPos += vSpace; 
   cp5.addTextlabel("roundStatusLabel")
      .setText("Round finished")
      .setPosition(centreStart, vPos+5)
      .setColorValue(0)
      .setFont(createFont("Arial",buttonFontSize*2))
      ;  
      
 /* vPos += vSpace;
  cp5.addButton("PracticeSession")
     .setBroadcast(false)
     .setValue(0)
     .setPosition(centreStart, vPos)
     .setSize(buttonW, buttonH)  
     .setBroadcast(true)
     .getCaptionLabel()
     .setFont(largeFont)
     .toUpperCase(false)
     .setSize(fSize-2)
     ;*/
}

public void preSetup(){
  colorMode(HSB);
  myFont = createFont("Courier New", 14);
  //startBX = width-(10+listBoxWidth);
  textFont(myFont);
  pfont = createFont("Arial",16,true); // use true/false for smooth/no-smooth
  largeFont = new ControlFont(pfont,241);
  cp5 = new ControlP5(this);
  portList = Serial.list();
  
}
public void initialiseControls(){
  setupLog();
  //setupListBoxes(20, 100, 300);
  //setupConnectButtons(20, 50, 300);
  //setupSerialDisplay();
  setupSerialDisplays(20, 50, 250, 50, numberOfUnits); //Xpos, ypos, x step, y step, quantity
  setupCommandButtons();
  if(numberOfEnvironments > 1) setupDiadControls(800, 35, 50);
  setupParticipentDataEntry( 1050, 35, 80, numberOfUnits);
  //setupPracticeButtons();
  setupManual(20, 50, 250, 50, numberOfUnits); //Xpos, ypos, x step, y step, quantity
  int buttonPos = 140;
  int buttonSpacing = 60;
  //buttonPos = setupAnimationButton(10, buttonPos, buttonSpacing);
  buttonPos = setupMouseConButton(10, buttonPos, buttonSpacing);
  buttonPos = setupRandomButtons(10, buttonPos, 350, 40);
  buttonPos = 60;
  buttonPos = setupParameters(10, buttonPos, buttonSpacing);
  buttonPos = setupTiming(10, buttonPos, buttonSpacing);
  buttonPos = setupSetButton(10, buttonPos, buttonSpacing);
  buttonPos = setupMessages(400, 180, buttonSpacing*2);
  if(numberOfEnvironments > 1) setupMapCon( 400, 40);
  setupTrackerCon(400+listBoxWidth+100, 40);
}


//The main event handler - called for EVERY event
public void controlEvent(ControlEvent theControlEvent) {
  if (theControlEvent.isTab()) {
    //println("got an event from tab : "+theControlEvent.getTab().getName()+" with id "+theControlEvent.getTab().getId());
  }
  serialEventHandler(theControlEvent);
  manualEventHandler(theControlEvent);
  if( numberOfEnvironments > 1 && theControlEvent.isFrom( cp5.getController( "gameMapping" ) ) ){
    Trial.setMapping((int) cp5.getController( "gameMapping" ).getValue());
    //User[n].portIndex = (int) cp5.getController( "gameMapping" ).getValue();
    //println("Game mapping set to: "+(int) cp5.getController( "gameMapping" ).getValue());
  }
  if( theControlEvent.isFrom( cp5.getController( "trackerballMapping" ) ) ){
    
    //println("Trackermode set:");
    for(int n = 0; n < numberOfUnits; n++){
      User[n].trackerballMode = (int)cp5.getController( "trackerballMapping" ).getValue();
      //print("Unit ");
      //print(n);
      //print(" Mode ");
      //println(User[n].trackerballMode);

    }
  }
}

//Check and handle events generated by the serial control UI
public void serialEventHandler(ControlEvent aControlEvent){
  for(int n = 0; n < numberOfUnits; n++){
    if( aControlEvent.isFrom( cp5.getController( serialConnectID.get(n) ) ) ){
      //println("SerialHandler Connect: "+serialConnectID.get(n));
      if(cp5.getController( serialConnectID.get(n) ).getValue() == 0.0f ) User[n].disconnectPort();
      else User[n].connectPort(this);
    }
    if( aControlEvent.isFrom( cp5.getController( serialListID.get(n) ) ) ){
      //println("SerialHandler List: "+serialListID.get(n));
      User[n].portIndex = (int) cp5.getController( serialListID.get(n) ).getValue();
      println("Port Index of unit: "+serialListID.get(n) + " is " + User[n].portIndex);
    }
  }
}

//Check and handle events generated by the manual control UI
public void manualEventHandler(ControlEvent aControlEvent){
  for(int n = 0; n < numberOfUnits; n++){
    if( aControlEvent.isFrom( cp5.getController( manualLEDID.get(n) ) ) ){
      println("Manual LED : "+manualLEDID.get(n));
      
       println("Value: " + cp5.getController( manualLEDID.get(n) ).getValue());
      if(cp5.getController( manualLEDID.get(n) ).getValue() == 1.0f ) User[n].sendLEDOn();
      else User[n].sendLEDOff();
    }
    if( aControlEvent.isFrom( cp5.getController( manualBeeperID.get(n) ) ) ){
      println("Manual Beep: "+manualBeeperID.get(n));
       println("Value: " + cp5.getController( manualBeeperID.get(n) ).getValue());
      if(cp5.getController( manualBeeperID.get(n) ).getValue() == 1.0f ) User[n].sendBeepOn();
      else User[n].sendBeepOff();
    }
    if( aControlEvent.isFrom( cp5.getController( manualVibID.get(n) ) ) ){
      println("Manual Vib: "+manualVibID.get(n));
       println("Value: " + cp5.getController( manualVibID.get(n) ).getValue());
      if(cp5.getController( manualVibID.get(n) ).getValue() == 1.0f ) User[n].setHaptics(1023, 1023);
      else User[n].setHaptics(0, 0);
    }
  }
}

public void setupTabs(){
  
  cp5.addTab("Configuration")
     .setColorBackground(color(100))
     .setColorLabel(color(255))
     .setColorActive(color(0))
     ;
   cp5.addTab("Serial")
     .setColorBackground(color(100))
     .setColorLabel(color(255))
     .setColorActive(color(0))
     ;    
   cp5.addTab("Manual")
     .setColorBackground(color(100))
     .setColorLabel(color(255))
     .setColorActive(color(0))
     ; 
  // if you want to receive a controlEvent when
  // a  tab is clicked, use activeEvent(true)
  
  cp5.getTab("default")
     .activateEvent(true)
     .setColorBackground(color(100))
     .setColorLabel(color(255))
     .setColorActive(color(0))
     .setLabel("Trials")
     .setWidth(150)
     .setHeight((25))
     .setId(1)
     ;

  cp5.getTab("Configuration")
     .activateEvent(true)
     .setLabel("Settings")
     .setWidth(150)
     .setHeight((25))
     .setId(2)
     ;

  cp5.getTab("Serial")
     .activateEvent(true)
     .setLabel("Serial Ports")
     .setWidth(150)
     .setHeight((25))
     .setId(3)
     ;

  cp5.getTab("Manual")
     .activateEvent(true)
     .setLabel("Manual Control")
     .setWidth(150)
     .setHeight((25))
     .setId(4)
     ;

}
  public void settings() {  size(1280, 700); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "PerceptualCrossingApp" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
