#include <18F458.h>
#device ADC=10

#FUSES NOWDT                 	//No Watch Dog Timer

#use delay(crystal=20000000)
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=PORT1,FORCE_SW)


// Pin aliases
#define LED_RED   PIN_B6
#define LED_GREEN PIN_B7
#define PIN_TENS  PIN_E1
#define PIN_UNITS PIN_E0

// Define serial messages
#define MSG_PEOPLE "$NB*%d\1"
#define MSG_ALARM  "$ALRM\1"


// Serial flags
const int FLAG_RESET   = 1;
const int FLAG_RESEND  = 2;
const int FLAG_SET_MAX = 4;