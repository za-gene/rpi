#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
//#include "hardware/xosc.h"
//#include "hardware/structs/rosc.h"

//#include "debounce.h"
#include "pace.h"


#define SW  20
#define SPK 19 // Speaker where we output noise
#define LED  25 // GPIO of built-in LED


//#define ALARM_NUM 0
//#define ALARM_IRQ TIMER_IRQ_0

typedef uint8_t u8;
typedef uint32_t u32;

//Debounce btn(20);

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


unsigned int slice_num;
volatile float vc =0.0;
volatile float fc = 400; // cut-off frequency (Hz)
volatile auto const sample_freq = 40'000;
volatile u32 repeat_us = 1'000'000 / sample_freq;
volatile float dt = 1.0 / sample_freq;
volatile float K = 2.0 * 3.1412 * fc * dt;

static void alarm_irq(void);

float filter()
{
	volatile float va = random() % 256;
	//printf("va = %f\n", (double)va);
	vc = vc + K * (va - vc);
	//volatile u8 on = vc >= 0.5;
	//printf("vc = %f, va = %f, on = %d\n", (double) vc, (double)va, on);
	return vc;
}

volatile u32 took = 0;




static void pwm_isr(void) 
{
	static volatile int count =0;
	//start("my callback");
	took =  time_us_32();
	volatile float va = random() % 256;
	vc = vc + K * (va - vc);

	if(gpio_get(SW)) {
		pwm_set_gpio_level(SPK, vc);
		gpio_put(LED, 1);
	} else {
		pwm_set_gpio_level(SPK, va);
		gpio_put(LED, 0);
	}

	if((++count % 20000) == 0) {
		count = 0;
	}

	//stop();
	took = time_us_32() - took;


	pwm_clear_irq(slice_num);
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


	const int top = 255;
	//constexpr float divisor = pace_pwm_divider(sample_freq, top);
	//static_assert(1.0 <= divisor && divisor < 256.0);
	int err = pace_config_pwm_irq(&slice_num, SPK, sample_freq, top, pwm_isr);
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

	while(1);

	while(1) {
		if(took) {
			printf("It took %d\n", took);
			took = 0;
		}
	}

	return 0;
}

