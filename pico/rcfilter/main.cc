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


float vc =0;
float fc = 400; // Hz
auto const sample_freq = 40'000;
float dt = (float)1'000'000 / sample_freq;
float K = 2.0 * 3.1412 * fc * dt;

bool filter()
{
	float va = (float)(random() & 1);
	vc = vc + K * (va - vc);
	return vc >= 0.5;
}

bool callback(struct repeating_timer *t)
{
	volatile u8 on;
	/*
	if(use_random) {
		gpio_put(LED, 1);
		on = random() &1;
	} else {
		gpio_put(LED, 0);
		on = rosc_hw->randombit;
	}
	*/
	on = filter();

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
	
	start("Generating 10,000 filtered samples");
	for(int i = 0; i < 10'000; ++i) {
		filter();
	}
	stop();
	printf("cf 10,000  * dt = %f\n", (double) dt * 10'000);




	struct repeating_timer timer;
	add_repeating_timer_us(dt, callback, 0, &timer);

	while(1) {
		if(btn.falling())
			use_random = 1- use_random;
	}





	return 0;
}

