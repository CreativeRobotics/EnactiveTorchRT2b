//configuration options for the trial

int setupMessages(int xp, int yp, int gap){
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

void setupTrackerCon(int xp, int yp){
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
void setupMapCon(int xp, int yp){
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
  
  
  
int setupSetButton(int xp, int yp, int gap){
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
int setupParameters(int xp, int yp, int gap){
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


int setupTiming(int xp, int yp, int gap){
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
