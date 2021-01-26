#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

using  u64 = uint64_t;

#define SPK 15

u64 count = 0;
u64 invert_every = 0;
bool repeating_timer_callback(struct repeating_timer *t) {
	static bool hi = false;
	if(invert_every == 0) return true;
	if((count++ % invert_every) == 0) {
		gpio_put(SPK, hi);
		hi = ! hi;
	}
	return true;
}


int main() 
{
	stdio_init_all();
	printf("freq-adj started\n");

	gpio_init(SPK);
	gpio_set_dir(SPK, GPIO_OUT);

	struct repeating_timer timer;

	//long int us = 1'000'000UL/8000/2;
	// negative means regardless of how long it took to execute
	add_repeating_timer_us(-10, repeating_timer_callback, NULL, &timer);


	for(;;) {
		int c = getchar();
		putchar(c);
		if( 'a' <= c && c <= 'z' ) {
			u64 freq = (c-'a') * (4000-100)/26 + 100; // scale notes from 100Hz to 4000Hz
			invert_every = 1'000'000UL/freq/2/10;
		}
		if( c == ' ') invert_every = 0;
	}

	return 0;
}

