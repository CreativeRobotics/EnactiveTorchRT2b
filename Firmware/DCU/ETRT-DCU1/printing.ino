void printDeviceState(Commander &Cmdr){
  
  Cmdr.println(settings.DCU_FW_VERSION);
  Cmdr.println(getTimestamp());
  Cmdr.print("BatVoltage: "); Cmdr.println(device.batteryVoltage);

  Cmdr.print("Q:");
  Cmdr.print(DCU_Q0, settings.dataPrecision);
  Cmdr.print(',');
  Cmdr.print(DCU_Q1, settings.dataPrecision);
  Cmdr.print(',');
  Cmdr.print(DCU_Q2, settings.dataPrecision);
  Cmdr.print(',');
  Cmdr.println(DCU_Q3, settings.dataPrecision);
  //Cmdr.print("Q0: "); Cmdr.println(DCU_Q0);
  //Cmdr.print("Q1: "); Cmdr.println(DCU_Q1);
  //Cmdr.print("Q2: "); Cmdr.println(DCU_Q2);
  //Cmdr.print("Q3: "); Cmdr.println(DCU_Q3);
  Cmdr.print("YPR:");
  Cmdr.print(DCU_YAW, settings.dataPrecision);
  Cmdr.print(',');
  Cmdr.print(DCU_PITCH, settings.dataPrecision);
  Cmdr.print(',');
  Cmdr.println(DCU_ROLL, settings.dataPrecision);
  //Cmdr.print("Yaw: "); Cmdr.println(DCU_YAW);
  //Cmdr.print("Pitch: "); Cmdr.println(DCU_PITCH);
  //Cmdr.print("Roll: "); Cmdr.println(DCU_ROLL);

  Cmdr.print("Gxyz:");
  Cmdr.print(DCU_GX, settings.dataPrecision);
  Cmdr.print(',');
  Cmdr.print(DCU_GY, settings.dataPrecision);
  Cmdr.print(',');
  Cmdr.println(DCU_GZ, settings.dataPrecision);
  //Cmdr.print("Gx: "); Cmdr.println(DCU_GX);
  //Cmdr.print("Gy: "); Cmdr.println(DCU_GY);
  //Cmdr.print("Gz: "); Cmdr.println(DCU_GZ);
  Cmdr.print("Axyz:");
  Cmdr.print(DCU_AX, settings.dataPrecision);
  Cmdr.print(',');
  Cmdr.print(DCU_AY, settings.dataPrecision);
  Cmdr.print(',');
  Cmdr.println(DCU_AZ, settings.dataPrecision);
  //Cmdr.print("Ax: "); Cmdr.println(DCU_AX);
  //Cmdr.print("Ay: "); Cmdr.println(DCU_AY);
  //Cmdr.print("Az: "); Cmdr.println(DCU_AZ);
  Cmdr.print("Mxyz:");
  Cmdr.print(DCU_MX, settings.dataPrecision);
  Cmdr.print(',');
  Cmdr.print(DCU_MY, settings.dataPrecision);
  Cmdr.print(',');
  Cmdr.println(DCU_MZ, settings.dataPrecision);
  
  //Cmdr.print("Mx: "); Cmdr.println(DCU_MX);
  //Cmdr.print("My: "); Cmdr.println(DCU_MY);
  //Cmdr.print("Mz: "); Cmdr.println(DCU_MZ);

  Cmdr.print("Compass Heading:   "); Cmdr.println(device.cHeading, settings.dataPrecision);
  Cmdr.print("IMU Initialised?:  "); device.IMU_Enabled        ? Cmdr.println("True") : Cmdr.println("False");
  Cmdr.print("IMU Errors:        "); Cmdr.println(device.IMU_SetupError);
  Cmdr.print("ESP Enabled?:      "); device.espEnabled         ? Cmdr.println("True") : Cmdr.println("False");
  Cmdr.print("ESP On Line?:      "); device.espOnline          ? Cmdr.println("True") : Cmdr.println("False");
  Cmdr.print("Server Enabled?:   "); device.espServerEnabled   ? Cmdr.println("True") : Cmdr.println("False");
  Cmdr.print("WiFi Connected?:   "); device.espWiFiConnected   ? Cmdr.println("True") : Cmdr.println("False");
  Cmdr.print("BT  Enabled?:      "); device.espBTEnabled       ? Cmdr.println("True") : Cmdr.println("False");
  Cmdr.print("UDP Enabled?:      "); device.espUDPEnabled      ? Cmdr.println("True") : Cmdr.println("False");
  Cmdr.print("Card Present?:     "); device.cardPresent        ? Cmdr.println("True") : Cmdr.println("False");
  Cmdr.print("Card Error?:       "); device.cardInitError      ? Cmdr.println("True") : Cmdr.println("False");
  Cmdr.print("Settings File OK?: "); device.cardSettingsFile   ? Cmdr.println("True") : Cmdr.println("False");
  Cmdr.print("Log BT?:           "); device.logBluetooth       ? Cmdr.println("True") : Cmdr.println("False");
  Cmdr.print("Log USB?:          "); device.logUSB             ? Cmdr.println("True") : Cmdr.println("False");
  Cmdr.print("Log SD?:           "); device.logSD              ? Cmdr.println("True") : Cmdr.println("False");

  Cmdr.print("Sensor:            "); Cmdr.println(device.sensorName);
  
  Cmdr.print("Aux Power:         "); device.v5PowerOn          ? Cmdr.println("ON") : Cmdr.println("OFF");
  Cmdr.print("Usr Switch:        "); device.userSwitchState    ? Cmdr.println("ON") : Cmdr.println("OFF");
  Cmdr.print("Usr Button:        "); device.userButtonState    ? Cmdr.println("ON") : Cmdr.println("OFF");
  
  Cmdr.println();
  //Cmdr.println(sonarInts);
}


void printDeviceSettings(Commander &Cmdr){
  Cmdr.print("IMU Initialised?:    "); device.IMU_Enabled        ? Cmdr.println("True") : Cmdr.println("False");
  Cmdr.print("ESP Enabled?:        "); device.espEnabled         ? Cmdr.println("True") : Cmdr.println("False");
  Cmdr.print("ESP On Line?:        "); device.espOnline          ? Cmdr.println("True") : Cmdr.println("False");
  Cmdr.print("Server Enabled?:     "); device.espServerEnabled   ? Cmdr.println("True") : Cmdr.println("False");
  Cmdr.print("WiFi Connected?:     "); device.espWiFiConnected   ? Cmdr.println("True") : Cmdr.println("False");
  Cmdr.print("Card Present?:       "); device.cardPresent        ? Cmdr.println("True") : Cmdr.println("False");
  Cmdr.print("Card Error?:         "); device.cardInitError      ? Cmdr.println("True") : Cmdr.println("False");
  Cmdr.print("Settings File OK?:   "); device.cardSettingsFile   ? Cmdr.println("True") : Cmdr.println("False");
  
  Cmdr.print("Log Quaternions?:    "); device.logQ             ? Cmdr.println("True") : Cmdr.println("False");
  Cmdr.print("Log YPR?:            "); device.logYPR           ? Cmdr.println("True") : Cmdr.println("False");
  Cmdr.print("Log Gyro?:           "); device.logGyro          ? Cmdr.println("True") : Cmdr.println("False");
  Cmdr.print("Log Accellerometer?: "); device.logAccel         ? Cmdr.println("True") : Cmdr.println("False");
  Cmdr.print("Log Magnetometer?:   "); device.logMag           ? Cmdr.println("True") : Cmdr.println("False");
  Cmdr.print("Log Heading?:        "); device.logHeading       ? Cmdr.println("True") : Cmdr.println("False");
  Cmdr.print("Log Inputs?:         "); device.logInputs        ? Cmdr.println("True") : Cmdr.println("False");
  Cmdr.print("Log Outputs?:        "); device.logOutputs       ? Cmdr.println("True") : Cmdr.println("False");
  
  Cmdr.print("Usr Switch:          "); device.userSwitchState    ? Cmdr.println("ON") : Cmdr.println("OFF");
  Cmdr.print("Usr Button:          "); device.userButtonState    ? Cmdr.println("ON") : Cmdr.println("OFF");
  
  Cmdr.print("IMU Errors:          "); Cmdr.println(device.IMU_SetupError);
}








//print functions for debugging

void USBMessage(String msg){
  USBS.print("DCU Status:");
  USBS.println(msg);
}
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
void printShortState(){
  DEBUG.println(getTimestamp());
  DEBUG.print("BatVoltage: "); DEBUG.println(device.batteryVoltage);
  DEBUG.print("Card Present?:        "); device.cardPresent   ? DEBUG.println("True") : DEBUG.println("False");
  DEBUG.print("Card Error?:          "); device.cardInitError ? DEBUG.println("True") : DEBUG.println("False");
  DEBUG.print("Settings File Found?: "); device.cardSettingsFile ? DEBUG.println("True") : DEBUG.println("False");
  DEBUG.print("Usr Switch: "); device.userSwitchState ? DEBUG.println("ON") : DEBUG.println("OFF");
  DEBUG.print("Usr Button: "); device.userButtonState ? DEBUG.println("ON") : DEBUG.println("OFF");
}

void printSensors(){
  /*
   * device.inputs[0] = Sensor.getRange();
  device.inputs[1] = Sensor.getDelta();
  device.inputs[2] = Sensor.getTOFRange();
  device.inputs[3] = Sensor.getTOFDelta();
  device.inputs[4] = Sensor.getAuxRange();
  device.inputs[5] = Sensor.getAuxDelta();
  */
  DEBUG.print("Range: ");
  DEBUG.print(device.inputs[0]);
  DEBUG.print(" DT: ");
  DEBUG.print(device.inputs[1]);
  DEBUG.print(" TOFR: ");
  DEBUG.print(device.inputs[2]);
  DEBUG.print(" TOFDT: ");
  DEBUG.print(device.inputs[3]);
  DEBUG.print(" AuxR: ");
  DEBUG.print(device.inputs[4]);
  DEBUG.print(" AuxDT: ");
  DEBUG.println(device.inputs[5]);
}
void printIMUData(){
  DEBUG.print("YPR: ");
  DEBUG.print(device.ypr[YAW]);
  DEBUG.print(":");
  DEBUG.print(device.ypr[PITCH]);
  DEBUG.print(":");
  DEBUG.println(device.ypr[ROLL]);
  
  DEBUG.print("Gyro");
  for(int n = 0; n < 3; n++){
    DEBUG.print(":");
    DEBUG.print(device.gyro[n]);
  }
  DEBUG.println();
  DEBUG.print("Accelerometer");
  for(int n = 0; n < 3; n++){
    DEBUG.print(":");
    DEBUG.print(device.accel[n]);
  }
  DEBUG.println();
}

void printTime(){
  printFMillis(millis()-device.clockLastMillis);  DEBUG.print(':');
  printFClock(device.seconds);  DEBUG.print(':');
  printFClock(device.minutes);  DEBUG.print(':');
  printFClock(device.hours);    DEBUG.print(' ');
  printFClock(device.day);      DEBUG.print(':');
  printFClock(device.month);    DEBUG.print(':');
  printFClock(device.year);
}

void printFClock(int number) {
  if (number < 10) {
    DEBUG.print('0'); // print a 0 before if the number is < than 10
  }
  DEBUG.print(number);
}

void printFMillis(int number) {
  if (number < 10) {
    DEBUG.print('0'); // print a 0 before if the number is < 10 
  }else if (number < 100) {
    DEBUG.print("00"); // print a 00 before if the number is < 100
  }
  DEBUG.print(number);
}

void printDeviceState(){

  DEBUG.println(getTimestamp());
  DEBUG.print("BatVoltage: "); DEBUG.println(device.batteryVoltage);
  
  DEBUG.print("Q0: "); DEBUG.println(DCU_Q0);
  DEBUG.print("Q1: "); DEBUG.println(DCU_Q1);
  DEBUG.print("Q2: "); DEBUG.println(DCU_Q2);
  DEBUG.print("Q3: "); DEBUG.println(DCU_Q3);
  
  DEBUG.print("Yaw: "); DEBUG.println(DCU_YAW);
  DEBUG.print("Pitch: "); DEBUG.println(DCU_PITCH);
  DEBUG.print("Roll: "); DEBUG.println(DCU_ROLL);

  
  DEBUG.print("Gx: "); DEBUG.println(DCU_GX);
  DEBUG.print("Gy: "); DEBUG.println(DCU_GY);
  DEBUG.print("Gz: "); DEBUG.println(DCU_GZ);

  DEBUG.print("Ax: "); DEBUG.println(DCU_AX);
  DEBUG.print("Ay: "); DEBUG.println(DCU_AY);
  DEBUG.print("Az: "); DEBUG.println(DCU_AZ);
  
  DEBUG.print("Mx: "); DEBUG.println(DCU_MX);
  DEBUG.print("My: "); DEBUG.println(DCU_MY);
  DEBUG.print("Mz: "); DEBUG.println(DCU_MZ);

  
  DEBUG.print("Compass Heading: "); DEBUG.println(device.cHeading);
  //printTime();
  DEBUG.print("Card Present?:        "); device.cardPresent   ? DEBUG.println("True") : DEBUG.println("False");
  DEBUG.print("Card Error?:          "); device.cardInitError ? DEBUG.println("True") : DEBUG.println("False");
  DEBUG.print("Settings File Found?: "); device.cardSettingsFile ? DEBUG.println("True") : DEBUG.println("False");

  

  DEBUG.print("Usr Switch: "); device.userSwitchState ? DEBUG.println("ON") : DEBUG.println("OFF");
  DEBUG.print("Usr Button: "); device.userButtonState ? DEBUG.println("ON") : DEBUG.println("OFF");
  
  DEBUG.println();
}
