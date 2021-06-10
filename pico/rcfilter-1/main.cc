#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/irq.h"
//#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "hardware/xosc.h"
#include "hardware/structs/rosc.h"

#include "debounce.h"

#define SW  20
#define SPK 19 // Speaker where we output noise
#define LED  25 // GPIO of built-in LED


#define ALARM_NUM 0
#define ALARM_IRQ TIMER_IRQ_0

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
volatile float fc = 400; // cut-off frequency (Hz)
volatile auto const sample_freq = 40'000;
volatile u32 repeat_us = 1'000'000 / sample_freq;
volatile float dt = 1.0 / sample_freq;
volatile float K = 2.0 * 3.1412 * fc * dt;

static void alarm_irq(void);

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

static void rearm() 
{
	// Clear the alarm irq
	hw_clear_bits(&timer_hw->intr, 1u << ALARM_NUM);

	//uint32_t delay_us = 2 * 1'000'000; // 2 secs
	// Alarm is only 32 bits so if trying to delay more
	// than that need to be careful and keep track of the upper
	// bits
	uint64_t target = timer_hw->timerawl + repeat_us;

	// Write the lower 32 bits of the target time to the alarm which
	// will arm it
	timer_hw->alarm[ALARM_NUM] = (uint32_t) target;
}

static void init_alarm()
{
	// Enable the interrupt for our alarm (the timer outputs 4 alarm irqs)
	hw_set_bits(&timer_hw->inte, 1u << ALARM_NUM);
	// Set irq handler for alarm irq
	irq_set_exclusive_handler(ALARM_IRQ, alarm_irq);
	// Enable the alarm irq
	irq_set_enabled(ALARM_IRQ, true);
	// Enable interrupt in block and at processor
	rearm();
}


/*
static void alarm_irq(void) 
{
	rearm();

	// Assume alarm 0 has fired
	printf("Alarm IRQ fired\n");
	alarm_fired = true;
}
*/

// 2021-05-31 appears not to be called. What the hell is going on?
// Ans: seems to be taking longer than allowed
//bool my_callback(struct repeating_timer *t)
static void alarm_irq(void) 
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
	//volatile float val = filter();

	volatile float va = random() & 1 ? 1.0 : 0.0;
	//printf("va = %f\n", (double)va);
	vc = vc + K * (va - vc);

	if(gpio_get(SW)) {
		gpio_put(SPK, vc>=0.5);
		gpio_put(LED, 1);
	} else {
		gpio_put(SPK, va == 1.0);
		gpio_put(LED, 0);
	}

	if((++count % 20000) == 0) {
		//printf(".");
		count = 0;
	}

	//stop();
	took = time_us_32() - took;


	//return false;
	//return true;
}

int main() 
{
	stdio_init_all();
	printf("\n--- START ---\n");

	gpio_init(SW);
	gpio_set_dir(SW, GPIO_IN);
	gpio_pull_up(SW);

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

	init_alarm();
	//struct repeating_timer timer;
	//
	//add_repeating_timer_us(repeat_us, my_callback, 0, &timer);
	//
	while(1);

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

