#include <test.h>
#define LED_PIN_ALARM PIN_B6
#define LED_PIN_GREEN PIN_B7
#define TENS_DISPLAY PIN_E1
#define UNITY_DISPLAY PIN_E0

boolean flag0 =0;
boolean flag1 =0;
#INT_EXT
void  EXT_isr(void) 
{
 flag0 =1;
}

#INT_EXT1
void  EXT1_isr(void) 
{
   flag1 =1;
   
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
   printf("*NB*%d",nb_persone);
   while(TRUE)
   {  
      output_high(LED_PIN_GREEN);
      bitsToBCD(nb_persone);
      
      delay_ms(250);
      
      if (flag0){
         flag0 =0;
         nb_persone++;
         printf("*NB*%d",nb_persone);
      }
      if (flag1){
         flag1 =0;
         nb_persone--;
         printf("*NB*%d",nb_persone);
      }
      
      if(nb_persone == 29){
         output_low(LED_PIN_GREEN);
         output_high(LED_PIN_ALARM);
         printf("ALARM ");
         delay_ms(2500);
         nb_persone =0;
         
         output_low(LED_PIN_ALARM );
      }
      

   }

}
