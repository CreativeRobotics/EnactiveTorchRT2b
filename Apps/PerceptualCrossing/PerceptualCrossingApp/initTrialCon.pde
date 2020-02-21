int buttonFontSize = 14;
String controlSubjectIDName = "SubjectNumber_";
void setupParticipentDataEntry(int xp, int yp, int gap, int quantity){
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


String getSubjectID(int unitNumber){
  if(unitNumber > numberOfUnits || unitNumber < 0) return " ";
  String controlName = controlSubjectIDName+String.valueOf(unitNumber);
  return cp5.get(Textfield.class, controlName).getText();
  //controlSubjectIDName
  //return " ";
}

int setupAnimationButton(int xp, int yp, int gap){
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

int setupMouseConButton(int xp, int yp, int gap){
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


int setupRandomButtons(int xp, int yp, int bWidth,int gap){
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

void setupLog(){
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

void setupDiadControls(int posX, int posY, int yGap){
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

void setupCommandButtons(){
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