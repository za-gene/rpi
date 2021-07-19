#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/adc.h"
//#include "hardware/clocks.h"
//#include "hardware/flash.h"
#include "hardware/gpio.h"
//#include "hardware/irq.h"
//#include "hardware/pwm.h"
//#include "hardware/spi.h"
// #include "tusb.h" // if you want to use tud_cdc_connected()
#include <inttypes.h>

#include "pi.h"
#include "debounce.h"

#define ALARM 0
#define DELAY 1'000


#define SIA 20
//Debounce sia(SIA, 1);
#define SIB 21
//Debounce sib(SIB, 1);
//#define BTN  22 // GPIO number, not physical pin
#define LED  25 // GPIO of built-in LED

volatile uint32_t count = 0;

// NB as of 2021-06-21 the GPIO parameter is ignored, and is enabled on any pin. According to SDK

volatile uint32_t ev_num = 0;

void sia_falling(uint gpio, uint32_t events)
{
	/*
	   if(gpio != SIA) return;
	   printf("Event %d:", ev_num++);
	   if(events & GPIO_IRQ_EDGE_FALL) {
	   puts("Falling");
	   }
	   if(events & GPIO_IRQ_EDGE_RISE) {
	   puts("Rising");
	   }
	   return;
	   */
	if(gpio_get(SIA) !=0) {
		count++;
		printf("Changed to = %d\n", count);
		pi_gpio_toggle(LED);
	}

	//if(!gpio_get(SIB) && (events == GPIO_IRQ_EDGE_RISE))
	//	count--;
}
void sia_rising(uint gpio, uint32_t events)
{
	if(!gpio_get(SIB)) 
		count--;
	printf("Decreased to = %d\n", count);
	pi_gpio_toggle(LED);
}

int gpio_is_high(uint gpio)
{
	if(gpio_get(gpio) == 0) return 0;
	return 1;
}

static void millis_irq()
{
	pi_alarm_rearm(ALARM, DELAY);
	volatile static uint64_t millis = 0;
	volatile static uint8_t prev_grate = gpio_is_high(SIA) * 0xFF;
	volatile static uint8_t cur_grate= prev_grate;
	volatile static int prev_state = (gpio_is_high(SIA) <<1) | gpio_is_high(SIB);
	volatile static int i = 0;
	volatile static int count = 0;
	if(millis % 1 == 0) {
		int cur_state = (gpio_is_high(SIA) <<1) | gpio_is_high(SIB);
		int prev_count = count;
		if(cur_state != prev_state) {
			//printf("cur_state %d (ms) = %d, prev = %d\n", i++, cur_state, prev_state);
			if((prev_state == 2) && (cur_state == 3)) {
				count ++;
				//puts("increase");
			}
			if((prev_state == 1) && (cur_state == 3)) {
				count--;
			}
			if(count != prev_count)
				printf("count %d = %d\n", i++, count);
			prev_state = cur_state;
		}
#if 0
		cur_grate = ( cur_grate << 1 ) & gpio_is_high(SIA);
		printf("grate = %d\n", cur_grate);
		if(cur_grate == 0 && prev_grate != 0) {
			puts("sia fell to 0");
		}
		if(cur_grate == 0xFF && prev_grate != 0xFF) {
			puts("sia went high");
		}
		prev_grate = cur_grate;
#endif
	}


	if(millis%1000==0) {
		//puts("hi");
		pi_gpio_toggle(LED);
	}

	millis++;

	//printf("Alarm IRQ fired %d\n", i++);
}

int main() 
{
	stdio_init_all();
	// while(!tud_cdc_connected()) sleep_ms(250); // wait for usb serial 

	pi_gpio_init(SIA, INPUT);
	pi_gpio_init(SIB, INPUT);
	pi_gpio_init(LED, OUTPUT);
	//pi_max7219_init();
	//pi_max7219_show_count(count);

	//pi_gpio_init(LED, OUTPUT);
	//pi_gpio_init(BTN, INPUT_PULLUP);
	//gpio_set_irq_enabled_with_callback(SIA, GPIO_IRQ_EDGE_FALL, true, &sia_falling);
	//gpio_set_irq_enabled_with_callback(SIB, GPIO_IRQ_EDGE_RISE, true, &sia_falling);
	//gpio_set_irq_enabled_with_callback(SIA, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &sia_falling);
	//gpio_set_irq_enabled_with_callback(SIA, GPIO_IRQ_EDGE_RISE, true, &sia_rising);

	pi_alarm_init(ALARM, millis_irq, DELAY);

	int i = 0;
	int count = 0;
	while(1) {
#if 0
		volatile static auto prev_state = (gpio_is_high(SIA) <<1) | gpio_is_high(SIB);
		auto cur_state = (gpio_is_high(SIA) <<1) | gpio_is_high(SIB);
		if(cur_state != prev_state) {
			printf("cur_state %d = %d\n", i++, cur_state);
			prev_state = cur_state;
		}
#endif
		continue;
		/*
		if(sia.falling()) {
			if(gpio_get(SIB)) {
				count++;
				//else
				//	count--;

				printf("ncreasing %d\n", count);
			}
		}

		if(sib.falling()) {
			if(gpio_get(SIA)) {
				count--;
				printf("decreasing %d\n", count);
			}
		}


		//pi_max7219_show_count(count);
		//busy_wait_us_32(100*1000);
		*/
	}

	return 0;
}

