//const int 

/*
 * trackX_A ADC2
 * trackX_B ADC3
 * trackY_A ADC4
 * trackY_B ADC5
 * Button SWCLK
 * Beep SWDIO
 * 
 */
const int BIN_LED_PIN = PB01;

const int XA_Pin      = PB05;
const int YA_Pin      = PB07;
const int XB_Pin      = PB06;
const int YB_Pin      = PB09;
const int button_Pin  = PA30;
const int beep_Pin    = PA31;


#define TRACKERBALL_XA XA_Pin
#define TRACKERBALL_XB XB_Pin
#define TRACKERBALL_YA YA_Pin
#define TRACKERBALL_YB YB_Pin
#define USER_SWITCH_PIN button_Pin
#define SOUNDER_PIN beep_Pin
