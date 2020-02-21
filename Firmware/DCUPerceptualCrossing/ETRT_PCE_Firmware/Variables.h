
#define LCDOK

#define USB Serial
#define SERIAL_SPEED 115200
#define COMMA   ','
#define NEWLINE '\n'
#define RETURN  '\r'
#define BUFFERSIZE 128

uint16_t environmentSize = 600;
float posToAngle = 0.0104719755;
uint8_t screenRotation = 3;
int tftXCursor = 2, tftYCursor = 10;
int tftTextSize = 3;

uint16_t displayWidth, displayHeight, displayXCentre, displayYCentre;
bool displayData = true;
bool animateData = false;
unsigned long nextDisplayUpdate = 0;
unsigned long displayUpdateInterval = 100;
bool streamData = false;
unsigned long nextPrintTime = 0;
unsigned long printInterval = 100;

bool trackerPaused = false;
bool swapXY = false;
volatile int8_t trackXDirection = 1;
volatile int8_t trackYDirection = 1;

bool peripheralPowerOn = false;
bool v5PowerOn = false;
char serialBuffer[BUFFERSIZE];
int bufferIndex = 0;
boolean newData = 0;
#define SERIAL_COMMAND 0
unsigned long packetsReceived = 0;

volatile long xCountTotal = 0;
volatile long yCountTotal = 0;
long oldXCount = 0;
long oldYCount = 0;

unsigned long nextLEDUpdate = 0;
unsigned long ledUpdateInterval = 500;

//elapsedMillis printTimer = 0;
unsigned long printTime = 50;

unsigned long dataSendTimer = 0;
boolean buttonState = 0;
boolean oldButtonState = 0;
boolean ledState = 0;
boolean displayOldButtonState = 0;
int LEDBrightness = 0;

//Haptics haptic1;
//Haptics haptic2;

int hapticRate1 = 0, hapticRate2 = 0;
//LCD
uint8_t nRows = 2;      //number of rows on LCD
uint8_t nColumns = 16;  //number of columns
char LCDText[33];
String displayMessage = "X";


bool beepState = 0;
bool beepActive = 0;
unsigned long beepStart = 0, beepEnd = 0;
unsigned long flashStart = 0, flashEnd = 0;
bool flashActive = 0;
bool manualFlash = 0; //overrides the button

bool drawTheAgent = true;
unsigned long beepLengthCommand = 0;
unsigned long flashLengthCommand = 0;
#define LRED 0
#define LGREEN 1
#define LBLUE 2
uint8_t randomLED[3] = {128,128,128};
const uint8_t whiteLED[3] = {128,128,128};
const uint8_t redLED[3] = {200,0,0};
const uint8_t greenLED[3] = {0,200,0};
const uint8_t blueLED[3] = {0,0,200};
const uint8_t purpleLED[3] = {200,0,200};
const uint8_t orangeLED[3] = {200,100,0};
const uint8_t yellowLED[3] = {200,200,0};
