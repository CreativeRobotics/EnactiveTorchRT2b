
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





void initConfig(){
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
