#include <test.h>
#define LED_PIN_ALARM PIN_B6
#define LED_PIN_GREEN PIN_B7
#INT_EXT
#define TENS_DISPLAY PIN_E1
#define UNITY_DISPLAY PIN_E0
boolean flag0 =0;
boolean flag1 =0;

void  EXT_isr(void) 
{

}

#INT_EXT1
void  EXT1_isr(void) 
{

}
void bitsToBCD(int value) {
   int tens = value / 10 % 10;
   int unity = value % 10;
   output_high(UNITY_DISPLAY);
   output_low(TENS_DISPLAY);
   output_d(tens);
   output_high(TENS_DISPLAY);
   
   delay_ms(10);
   output_low(UNITY_DISPLAY);
   output_d(unity);
   output_low(UNITY_DISPLAY);
   delay_ms(10);
}
void main()
{
   int nb_persone =0;
   enable_interrupts(INT_EXT);
   enable_interrupts(INT_EXT1);
   enable_interrupts(GLOBAL);
   setup_low_volt_detect(FALSE);

   while(TRUE)
   {  
      output_high(LED_PIN_GREEN);
      bitsToBCD(nb_persone);
      delay_ms(250);
      output_low(LED_PIN_GREEN);
      nb_persone++;
      if(nb_persone == 99){
         output_high(LED_PIN_ALARM);
         delay_ms(250);
         nb_persone =0;
         output_low(LED_PIN_ALARM );
      }
      

   }

}
