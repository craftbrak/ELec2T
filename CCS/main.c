#include "common.h"

int flag_add = 0;
int flag_sub = 0;
int flag_data = 0;

char buffer[BUFFER_SIZE];

int max_people = 5;

#int_EXT
void  EXT_isr(void)
{
   flag_add = 1;
}

#int_EXT1
void  EXT1_isr(void)
{
   flag_sub = 1;
}

#int_RDA
void  RDA_isr(void)
{
   int i = 0;
   flag_data = 1;
   //for (i = 0; kbhit() && i < BUFFER_SIZE; ++i) {
   do {
     buffer[i++] = getc();
   } while (i < BUFFER_SIZE);
   output_d(i);
}

void send(int armed, int people, int max_people) {
   // printf("%c%c%c", (char) armed, (char) people, (char) max_people);
   putc((char) armed);
   putc((char) people);
   putc((char) max_people);
}

void display(int value) {
   int unit, ten;
   unit = ((value / 10) % 10) & 0xf;
   ten  = (value % 10) & 0xf;

   output_high(PIN_E0);
   output_low(PIN_E1);

   //output_c(((value / 10) % 10) & 0xf);
   output_bit(PIN_C0, (unit >> 0) & 1);
   output_bit(PIN_C1, (unit >> 1) & 1);
   output_bit(PIN_C2, (unit >> 2) & 1);
   output_bit(PIN_C3, (unit >> 3) & 1);


   output_high(PIN_E1);
   output_low(PIN_E0);

   //output_c((value % 10) & 0xf);
   output_bit(PIN_C0, (ten >> 0) & 1);
   output_bit(PIN_C1, (ten >> 1) & 1);
   output_bit(PIN_C2, (ten >> 2) & 1);
   output_bit(PIN_C3, (ten >> 3) & 1);

   output_high(PIN_E1);
   output_high(PIN_E0);
}

void main()
{
   int people = 0;
   int armed = 0;
   int last = 0;
   int flags;

   setup_adc_ports(NO_ANALOGS);
   setup_adc(ADC_OFF);
   setup_psp(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   setup_wdt(WDT_OFF);
   setup_timer_0(RTCC_INTERNAL);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   enable_interrupts(INT_EXT);
   enable_interrupts(INT_EXT1);
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);
   //set_tris_c(0x80);
//Setup_Oscillator parameter not selected from Intr Oscillotar Config tab

   send(armed, people, max_people);
   for (;;) {
      if (flag_add) {
         flag_add = 0;
         ++people;
      }
      if (flag_sub) {
         flag_sub = 0;
         if (people > 0)
            --people;
      }
      if (flag_data) {
         flag_data = 0;
         flags = (int) buffer[0];
         //output_d(flags);

         if (flags & FLAG_SET_MAX)
            max_people = buffer[1];

         if (flags & FLAG_RESEND)
            send(armed, people, max_people);

         if (flags & FLAG_RESET)
            people = 0;
      }

      armed = people >= max_people;
      if (armed) {
         output_low(PIN_B7);
         output_toggle(PIN_B6);
      } else {
         output_low(PIN_B6);
         output_high(PIN_B7);
      }

      if (last != people) {
         last = people;
         display(people);
         send(armed, people, max_people);
      }

      delay_ms(200);
   }

}
