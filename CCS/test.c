#include <test.h>

#INT_EXT
boolean flag0 =0;
boolean flag1 =0;

void  EXT_isr(void) 
{

}

#INT_EXT1
void  EXT1_isr(void) 
{

}

void main()
{
   int val =0;
   enable_interrupts(INT_EXT);
   enable_interrupts(INT_EXT1);
   enable_interrupts(GLOBAL);
   setup_low_volt_detect(FALSE);

   while(TRUE)
   {
      output_d(val);
      delay_ms(250);
      val++;
      if(val == 99){
         val =0;
      }

   }

}
