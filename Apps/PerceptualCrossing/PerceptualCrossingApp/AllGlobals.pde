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
color backgroundCol = color(220);
color tabBackgroundCol = color(170);

color backgroundColTRIAL_ACTIVE = color(255, 197, 110);
color tabBackgroundColTRIAL_ACTIVE = color(50);

color backgroundColROUND_ACTIVE = color(247, 98, 98);
color tabBackgroundColROUND_ACTIVE = color(50);
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

color buttonNormal             = color(128); //mid grey
color buttonMouseover          = color(200); //light grey
color buttonActive             = color(128,0,0); //mid red
color buttonActiveMouseover    = color(200,0,0); //bright red

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
