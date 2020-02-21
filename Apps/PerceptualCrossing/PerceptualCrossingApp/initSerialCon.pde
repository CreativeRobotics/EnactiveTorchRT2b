
//int startAX = 10;
//int startBX = 10; //initialised with window width in startup
//int startAY = 10;
//int startBY = 10;

StringList serialConnectID;
StringList serialListID;

void setupSerialDisplays(int px, int py, int gapX, int gapY, int quantity){
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

void setupConnectButtons(int px, int py, int gap){
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


void setupListBoxes(int px, int py, int gap){
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