#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "hardware/xosc.h"
#include "hardware/structs/rosc.h"

#include "debounce.h"

#define SPK 19 // Speaker where we output noise
#define LED  25 // GPIO of built-in LED


typedef uint8_t u8;
typedef uint32_t u32;

Debounce btn(20);

volatile u8 use_random = 1;

u32 start_time;
void start(const char* str)
{
	printf("%s ... ", str);
	start_time = time_us_32();
}

void stop()
{
	u32 now = time_us_32();
	printf("took %d us\n", now - start_time);
}


bool callback(struct repeating_timer *t)
{
	volatile u8 on;
	if(use_random) {
		gpio_put(LED, 1);
		on = random() &1;
	} else {
		gpio_put(LED, 0);
		on = rosc_hw->randombit;
	}

	gpio_put(SPK, on);
	return true;
}

int main() 
{
	stdio_init_all();

	gpio_init(SPK);
	gpio_set_dir(SPK, GPIO_OUT);
	gpio_init(LED);
	gpio_set_dir(LED, GPIO_OUT);

	// appears to be unnecessary
	//xosc_init(); // I think you need to enable the xosc before using random bit generator
	
	start("Generating 10,000 hardware bytes");
	for(int i = 0; i < 10'000; ++i) {
		volatile u8 b=0;
		for(int j = 0; j < 8; ++j) {
			b <<= 1;
			b |= (rosc_hw->randombit & 1);
		}
	}
	stop();

	start("Generating 10,000 random()s");
	for(int i = 0; i < 10'000; ++i) {
		volatile u8 b = random();
	}
	stop();




	auto const sample_freq = 40'000;
	struct repeating_timer timer;
	add_repeating_timer_us(1'000'000/sample_freq, callback, 0, &timer);

	while(1) {
		if(btn.falling())
			use_random = 1- use_random;
	}





	return 0;
}

