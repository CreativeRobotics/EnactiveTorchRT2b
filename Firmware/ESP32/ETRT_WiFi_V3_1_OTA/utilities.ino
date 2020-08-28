void debugPrint(String message){
  String msg = String("DEBUG:" + message);
  Serial.print(msg);
}
void debugPrintln(String message){
  String msg = String("DEBUG:" + message);
  Serial.println(msg);
}
void debugWrite(char ch){
  Serial.write(ch);
}
void debugStartLine(){
  Serial.print("DEBUG:");
}
void debugEndLine(){
  Serial.println("");
}

void configIntPin(){
  pinMode(COMMAND_ENABLE_PIN, INPUT_PULLUP);
  attachInterrupt(COMMAND_ENABLE_PIN, bootPin_ISR, CHANGE);
}

//Boot pin should be HIGH to disable bootloader, pulled low for boot loader at start.
//Use HIGH to enable commands, low when sending data for transmission
void bootPin_ISR(){
  device.serialCommandMode = digitalRead(COMMAND_ENABLE_PIN);
}
