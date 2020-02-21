
//update functions for the environments

void updateEnvironment(int envNo){
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
  void Start(int startPos){
    move(startPos);
  }
  
  //-----------------------------------------------------------------------
  
  void setWorldWidth(int newWWidth){
    worldW = newWWidth;
    staticPos = wrap(staticPos);
    playerPosition = wrap(playerPosition);
  }
  
  //-----------------------------------------------------------------------
  void setUserParameters(int uWidth, int lWidth, int lOffset, int sWidth, int sPos){
    setplayerWidth(uWidth);
    setLureWidth(lWidth);
    setLureOffset(lOffset);
    setStaticPosition(sPos);
    setStaticWidth(sWidth);
    move();
  }
  //-----------------------------------------------------------------------
  void setStaticParameters(int sWidth, int sPos){
    setStaticPosition(sPos);
    setStaticWidth(sWidth);
    
  }
  //-----------------------------------------------------------------------
  void setplayerWidth(int uWidth){
    playerWidth = uWidth;
  }
  //-----------------------------------------------------------------------
  void setLureWidth(int lWidth){
    lureWidth = lWidth;
  }
  //-----------------------------------------------------------------------
  void setLureOffset(int lOffset){
    lureOffset = lOffset;
  }
  //-----------------------------------------------------------------------
  void setStaticPosition(int sPos){
    staticPos = wrap(sPos);
  }
  //-----------------------------------------------------------------------
  void setStaticWidth(int sWidth){
    staticWidth = sWidth;
  }
  //-----------------------------------------------------------------------
  //impliment classes for returning positions based on a worldWidth argument ...
  int getUserStart(){
    return wrap(playerPosition);
  }
  //-----------------------------------------------------------------------
  int getUserEnd(){
    return wrap(playerPosition+playerWidth);
  }
  //-----------------------------------------------------------------------
  int getLureStart(){
    return wrap(playerPosition+playerWidth+lureOffset);
  }
  //-----------------------------------------------------------------------
  int getLureEnd(){
    return wrap(playerPosition+playerWidth+lureOffset+lureWidth);
  }
  //-----------------------------------------------------------------------
  int getStaticStart(){
    return wrap(staticPos);
  }
  //-----------------------------------------------------------------------
  int getStaticEnd(){
    return wrap(staticPos+staticWidth);
  }
  //-----------------------------------------------------------------------
  boolean isTouchingStatic(){
    //return true if the user is overlapping the static object
    return checkSelfOverlap();
  }
  //-----------------------------------------------------------------------
  boolean checkSelfOverlap() {
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
  int wrap(int inPosition) {
    
    int ps = inPosition;
    if(ps < 0) return (worldW+ps)% worldW;
    return inPosition % worldW;
    
    /*
    //wrap around the world limit
    if(inPosition < 0) return -inPosition % worldW;
    return inPosition % worldW;*/
  }
  //-----------------------------------------------------------------------
  void move() {
    playerPosition = wrap(playerPosition);
    lurePosition = wrap( playerPosition + playerWidth + lureOffset);
  }
  //-----------------------------------------------------------------------
  void move(int newPos) {
    playerPosition = wrap(newPos);
    lurePosition = wrap( playerPosition + playerWidth + lureOffset);
  }
  void shift(int newPos) {
    playerPosition = wrap(playerPosition+newPos);
    lurePosition = wrap( playerPosition + playerWidth + lureOffset);
  }
  
  void setHapticValues(int hapticVal){
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
  void start(int Player0StartPos, int Player1StartPos){
    Player0.Start(Player0StartPos);
    Player1.Start(Player1StartPos);
  }
  //-----------------------------------------------------------------------
  void setWorldWidth(int newW){
    worldWidth = newW;
    Player0.setWorldWidth(worldWidth);
    Player1.setWorldWidth(worldWidth);
  }
  //-----------------------------------------------------------------------
  //move by directly setting the user positions
  void updateMove(int newPosA, int newPosB){
    Player0.move(newPosA);
    Player1.move(newPosB);
    //update all the overlap checks
    checkCollisions();
    updateHaptics();
    //Player0SelfOverlap = Player0.isTouchingStatic(worldWidth);
    //Player1SelfOverlap = Player1.isTouchingStatic(worldWidth);
  }
  //-----------------------------------------------------------------------
  void update(){
    //update all the overlap checks
    checkCollisions();
    updateHaptics();
  }
  //-----------------------------------------------------------------------
  void update(PCInterfaceUnit user0, PCInterfaceUnit user1){
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
  void updateShift(int newPosA, int newPosB){
    Player0.shift(newPosA);
    Player1.shift(newPosB);
    //update all the overlap checks
    checkCollisions();
    updateHaptics();
    //Player0SelfOverlap = Player0.isTouchingStatic(worldWidth);
    //Player1SelfOverlap = Player1.isTouchingStatic(worldWidth);
  }
  //-----------------------------------------------------------------------
  int wrap(int inPosition) {
    //wrap around the world limit
    int ps = inPosition;
    if(ps < 0) return (worldWidth+ps)% worldWidth;
    return inPosition % worldWidth;
  }
  //-----------------------------------------------------------------------

  void checkCollisions(){
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
  
  void updateHaptics(){
    if(Player0Contact == true) Player0.setHapticValues(hapticContactValue);
    else                       Player0.setHapticValues(hapticNoContactValue);
    
    if(Player1Contact == true) Player1.setHapticValues(hapticContactValue);
    else                       Player1.setHapticValues(hapticNoContactValue);
    
  }
  //-----------------------------------------------------------------------
  
  boolean checkUserCrossing(PCPlayer Player0, PCPlayer user2){
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
  
  boolean checkLureCrossing(PCPlayer Player0, PCPlayer user2){
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
