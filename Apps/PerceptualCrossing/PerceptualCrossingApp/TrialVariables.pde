

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

void startTrial(){
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

void stopTrial(){
  
  println("Stopping Trial");
  //stop logging data
  stopLogs();
  Trial.stopTrial();
  cp5.get(Textlabel.class,"trialStatusLabel").setText("Practice Mode");
} 


void startRound(){
  //write a new file header for this round
  if(Trial.roundNumber > 0) writeFileHeader();
  practiceIterations = 0;
  
  println("Running Round ...");
  Trial.startRound();
  //Trial.roundActive = true;
  cp5.get(Textlabel.class,"RoundLabel").setText(String.valueOf(Trial.roundNumber));
  cp5.get(Textlabel.class,"roundStatusLabel").setText("Round Active");
}

void endRound(){
  println("Ending Round ...");
  cp5.get(Textlabel.class,"roundStatusLabel").setText("Round Finished");
  Trial.stopRound();
  //Trial.roundActive = false; //just for debugging
  //Trial.roundNumber++;
}
//=====================================================================================================================================================================
boolean checkExitKey(){
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
  
  void startTrial(){
    trialMode = TRIAL_ACTIVE;
    roundNumber = 0;
  }
  
  void stopTrial(){
    trialNumber++;
    roundNumber = 0;
    trialMode = FREE_PRACTICE;
  }
  
  void startRound(){
    //trialMode = TRIAL_ACTIVE;
    roundActive = true;
  }
  
  void stopRound(){
    //trialMode = FREE_PRACTICE;
    roundActive = false;
    roundNumber++;
  }
  
  
  void initialiseMapping(){
    //map each player to their neighbor
    for(int n = 0; n < numberOfGames; n++){
      playerPairs[n][0] = n;
      playerPairs[n][1] = numberOfGames+n;
    }
  }
  int getUsersGame(int userNo){
    //return the game that the given user is in
    for(int n = 0; n < numberOfGames; n++){
      if(playerPairs[n][0] == userNo || playerPairs[n][1] == userNo) return n;
    }
    return 0;
  }
  int getPlayerSlot(int userNo){
    //return the player slot (A or B) that the player is assigned to within their game
    //Returns 0 for user A and 1 for user B
    int game = getUsersGame(userNo);
    if(playerPairs[game][0] == userNo) return 0;
    else return 1;
  }
  int getUsersOpponent(int userNo){
    //return the game that the given user is in
    int game = getUsersGame(userNo);
    if(playerPairs[game][0] == userNo) return playerPairs[game][1];
    else return playerPairs[game][0];
  }
  
  int getPlayer0User(int environmentNo){
    //get the user assigned to player0 for this environment
      return playerPairs[environmentNo][0];
  }
  int getPlayer1User(int environmentNo){
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
  
  void setMapping(int mapNo){
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


void updatePlayerMappings(){
  
  
}

void updateTimingData(){
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
