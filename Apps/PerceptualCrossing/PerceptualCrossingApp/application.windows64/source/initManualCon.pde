
StringList manualLEDID;
StringList manualBeeperID;
StringList manualVibID;


void setupManual(int px, int py, int gapX, int gapY, int quantity){
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
