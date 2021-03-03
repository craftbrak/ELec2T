#include "common.h"
#include "display.h"

#define BUFFER_SIZE 3

typedef boolean bool;

bool flag_add = false;
bool flag_sub = false;
bool flag_dat = false;

char buffer[BUFFER_SIZE];
int max_people = 5;

void send(bool armed, int people, int max_people);

#INT_EXT
void EXT_isr(void) {
	flag_add = true;
}

#INT_EXT1
void EXT1_isr(void) {
	flag_sub = true;
}

#INT_RDA
void RDA_isr(void) {
	/*flag_dat = true;

	for (int i = 0; i < BUFFER_SIZE; ++i)
		buffer[i] = getc();

	send(false, 0, 0);*/
	flag_dat = true;
	buffer[0] = getc();
}

void send(bool armed, int people, int max_people) {
	putc((char) armed);
	putc((char) (people >> 8));
	putc((char) (people & 0xff));
	putc((char) (max_people >> 8));
	putc((char) (max_people & 0xff));
}

void main() {
	int people = 0;
	bool changed = false;
	int flags = 0;

	enable_interrupts(INT_EXT);
	enable_interrupts(INT_EXT1);
	enable_interrupts(INT_RDA);
	enable_interrupts(GLOBAL);
	setup_low_volt_detect(false);

	/*printf(MSG_PEOPLE, people);*/
	display(people);
	send(false, people, max_people);
	output_high(LED_GREEN);
	output_low(LED_RED);

	while(true) {
		if (flag_add) {
			flag_add = false;
			changed = !changed;
			++people;
		}
		if (flag_sub) {
			flag_sub = false;
			changed = !changed;
			--people;
		}
		if (flag_dat) {
			flag_dat = false;
			output_high(LED_RED);
			changed = false;

			/*flags = (int) buffer[0];
			changed = changed || flags;
			int value = (buffer[1] << 8) | buffer[2];

			if (flags & FLAG_SET_MAX)
				max_people = value;

			if (flags & FLAG_RESET)
				people = 0;*/
		}

		if (changed) {
			display(people);
			bool armed = people >= max_people;

			if (armed) {
				output_high(LED_RED);
				output_low(LED_GREEN);
			} else {
				output_low(LED_RED);
				output_high(LED_GREEN);
			}
			send(armed, people, max_people);
		}

		changed = false;
		flags = 0;
		delay_ms(200);
	}
}
