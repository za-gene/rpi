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


volatile float vc =0.0;
volatile float fc = 400; // Hz
volatile auto const sample_freq = 13'000;
volatile u32 repeat_us = 1'000'000 / sample_freq;
volatile float dt = 1.0 / sample_freq;
volatile float K = 2.0 * 3.1412 * fc * dt;

float filter()
{
	volatile float va = random() & 1 ? 1.0 : 0.0;
	//printf("va = %f\n", (double)va);
	vc = vc + K * (va - vc);
	//volatile u8 on = vc >= 0.5;
	//printf("vc = %f, va = %f, on = %d\n", (double) vc, (double)va, on);
	return vc;
}

volatile u32 took = 0;

// 2021-05-31 appears not to be called. What the hell is going on?
// Ans: seems to be taking longer than allowed
bool my_callback(struct repeating_timer *t)
{
	static volatile int count =0;
	//start("my callback");
	took =  time_us_32();
	//volatile u8 on;
	/*
	if(use_random) {
		gpio_put(LED, 1);
		on = random() &1;
	} else {
		gpio_put(LED, 0);
		on = rosc_hw->randombit;
	}
	*/
	volatile float val = filter();

	gpio_put(SPK, val>=0.5);

	if((++count % 20000) == 0) {
		//printf(".");
		count = 0;
	}

	//stop();
	took = time_us_32() - took;


	return false;
	return true;
}

int main() 
{
	stdio_init_all();
	printf("\n--- START ---\n");

	gpio_init(SPK);
	gpio_set_dir(SPK, GPIO_OUT);
	gpio_init(LED);
	gpio_set_dir(LED, GPIO_OUT);
#if 1
	start("Generating 10,000 filtered samples");
	for(int i = 0; i < 10'000; ++i) {
		volatile auto v = filter();
	}
	stop();
	printf("cf 10,000  * repeat_us = %d\n", 10000* repeat_us); //(double) dt * 10'000);
#endif

	for(int i = 0; i < 10; ++i) {
		//filter();
		printf("voltage %f\n", (double)filter());
	}
	printf("\n");


	//while(1);

	printf("repeat_us=%d\n", repeat_us);

	struct repeating_timer timer;
	add_repeating_timer_us(repeat_us, my_callback, 0, &timer);

	while(1) {
		if(took) {
			printf("It took %d\n", took);
			took = 0;
		}
	}

	while(1) {
		if(btn.falling())
			use_random = 1- use_random;
	}





	return 0;
}

