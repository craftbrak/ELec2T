#include "common.h"
#include "display.h"

void enable_tens(void) {
	output_high(PIN_UNITS);
	output_low(PIN_TENS);
}

void enable_units(void) {
	output_high(PIN_TENS);
	output_low(PIN_UNITS);
}

void disable_display(void) {
	output_high(PIN_TENS);
	output_high(PIN_UNITS);
}

void display(int value) {
	enable_tens();
	output_c((value / 10) % 10);
	enable_units();
	output_c(value % 10);
	disable_display();
}