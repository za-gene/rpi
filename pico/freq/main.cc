#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define SPK 15

bool repeating_timer_callback(struct repeating_timer *t) {
	static bool hi = false;
	gpio_put(SPK, hi);
	hi = ! hi;
	return true;
}


int main() 
{
	gpio_init(SPK);
	gpio_set_dir(SPK, GPIO_OUT);

	struct repeating_timer timer;

	constexpr int freq = 440;
	long int us = 1'000'000UL/freq/2;
	// negative means regardless of how long it took to execute
	add_repeating_timer_us(-us, repeating_timer_callback, NULL, &timer);

	for(;;);

	return 0;
}

