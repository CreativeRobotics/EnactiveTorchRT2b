/*
 edit: D:\Dropbox\Arduino\hardware\espressif\esp32\boards.txt
*/
#include <Commander.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <WiFiUdp.h>
#include "time.h"
#include <BluetoothSerial.h>
#include "ETRT_ESP_Globals.h"
Commander cmdESP;
extern const commandList_t  ESPCommands[]; //forward declare the master command list
extern const uint16_t       numOfESPCmds; //This is a forward declarationso the compiler knows we are going to declare this variable properly later



const char* host = "esp32";
const char* myssid   = "VM4789902";
const char* password = "vrtqvfDxr8hq";

int delayTime = 1000;
int nextTime = 0;

const char* ntpServer = "pool.ntp.org";

//UDP Settings:----------------------------------------------------------------------------------
//IP address to send UDP data to:
// either use the ip address of the server or 
// a network broadcast address
//const char * udpAddress = "192.168.0.255";
//const int udpPort = 3333;
//Are we currently connected?
boolean connected = false;
//The udp library class
WiFiUDP udp;


//Bluetooth Settings:----------------------------------------------------------------------------------


//#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
//#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
//#endif

BluetoothSerial SerialBlueTooth;

//AsyncWebServer server(80);
WiFiServer server(device.webServerPort);
WiFiServer telnetServer(device.telnetPort);
WebServer  wServer(device.webServerPort);
//----------------------------------------------------------------------------------
void setup()
{
  //SerialBlueTooth.register_callback(btCallback);
  
  Serial.begin(115200);
  cmdESP.setBuffer(256);
  cmdESP.begin(&Serial, ESPCommands, numOfESPCmds);
  cmdESP.commanderName = "ETRT-ESP32>";
  cmdESP.disableErrors();
  //delay(500);
  //cmdESP.printCommandPrompt();
  //cmdESP.println(numOfESPCmds);
  //cmdESP.printCommandPrompt();
  
  //for(int n = 0; n < WIFI_LIST; n++){
    //wifiLogins[n].ssid.reserve(32);
    //wifiLogins[n].pswd.reserve(32);
  //}
  //wifiLogins[0].ssid = "VM4789902";
  //wifiLogins[0].pswd = "vrtqvfDxr8hq";

  device.btName.reserve(32);
  //webFormSubmit.reserve(256); //reserve a buffer
  //initSerialPort();
    //SerialBT.begin("ETRT_V2"); //Bluetooth device name
    
    //pinMode(LEDPIN, OUTPUT);      // set the LED pin mode

    //Get the configuration details from the main processor
    //Send get commands over serial and unpack the results
  delay(10);
  
    //Serial.println("The device started, now you can pair it with bluetooth!");
    // We start by connecting to a WiFi network
    //Serial.println();
    //Serial.println();
  Serial.println("ESP Status: ONLINE");
}
//----------------------------------------------------------------------------------
int value = 0;

void loop(){
  cmdESP.update();
  if(device.OTARunning){
    checkOTA();
  }else updateNormal();
}

void updateNormal(){
  checkInit();
  serviceBT();
  checkWiFi();
  checkTelnet();
  if(device.serverSending){
    if(millis() > device.clientDisconnectTime){
      //timeout after x millis so close the connection to keep things tidy
      sendToClient(blankPage);
      endClient();
    }
  }
}

void checkInit(){
  //check if any services need starting
  if(device.connectToWiFi == true){
    device.connectToWiFi = false;
    if(connectWiFi()){
      //Serial.println("ESP: WiFi Connected");
      Serial.println("ESP Status: WiFi Connected");
      Serial.print("ESP Status: WiFiIP=");
      Serial.println(WiFi.localIP());
      device.wifiConnected = true;
      initTime(device.timeServer);
    }else Serial.println("ESP Status: WiFiConnErr");
  }
  if(device.startServer == true){
    device.startServer = false;
    enableServer();
    
    Serial.print("ESP Status: Server Enabled on port ");
    Serial.println(device.webServerPort);
    //Serial.print("ESP Status: WiFiIP=");
    //Serial.println(WiFi.localIP());
      //Serial.println("ESP Status: WiFiConnErr");
    //}
  }
  if(device.startTelnet == true){
    device.startTelnet = false;
    enableTelnet();
    
    Serial.print("ESP Status: Telnet Enabled on port ");
    Serial.println(device.telnetPort);
    //Serial.print("ESP Status: WiFiIP=");
    //Serial.println(WiFi.localIP());
      //Serial.println("ESP Status: WiFiConnErr");
    //}
  }
  if(device.startBT == true){
    device.startBT = false;
    startBlueTooth();
    Serial.println("ESP Status: Bluetooth Enabled");
    Serial.print("ESP Status: BlueTooth=");
    Serial.println(device.btName);
    device.btEnabled = true;
  }
  if(device.startUDP == true){
    device.startUDP = false;
    startUDP();
    Serial.println("ESP Status: UDP Enabled");
    Serial.println(device.btName);
    device.udpEnabled = true;
  }
  if(device.startOTA == true){
    device.startOTA = false;
    if( enableOTA() ){
      Serial.println("ESP Status: OTA Started");
    }
  }
}
