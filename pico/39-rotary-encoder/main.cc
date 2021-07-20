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
#include "pulse.h"

#define ALARM 0
#define DELAY 1'000


#define SIA 	21
//Debounce sia(SIA, 1);
#define SIB 	20
#define SW	19 
//Debounce sib(SIB, 1);
//#define BTN  22 // GPIO number, not physical pin
#define LED  25 // GPIO of built-in LED

volatile uint32_t count = 0;


volatile uint32_t ev_num = 0;


int gpio_is_high(uint gpio)
{
	if(gpio_get(gpio) == 0) return 0;
	return 1;
}

static void millis_irq()
{
	pi_alarm_rearm(ALARM, DELAY);
	volatile static uint64_t millis = 0;
	//volatile static uint8_t prev_grate = gpio_is_high(SIA) * 0xFF;
	//volatile static uint8_t cur_grate= prev_grate;
	volatile static int i = 0;
	volatile static int count = 0;
	static auto state = [](){return (gpio_is_high(SIB) <<1) | gpio_is_high(SIA); };
	volatile static int prev_state = state();
	if(millis % 1 == 0) {
		int cur_state = state();
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
	}

#if 0
	if(millis % 500 == 0) {
		//puts("hi");
		pi_gpio_toggle(LED);
	}
#endif
	millis++;

}

int main() 
{
	stdio_init_all();
	// while(!tud_cdc_connected()) sleep_ms(250); // wait for usb serial 

	pi_gpio_init(SIA, INPUT);
	pi_gpio_init(SIB, INPUT);
	pi_gpio_init(LED, OUTPUT);
	pi_alarm_init(ALARM, millis_irq, DELAY);

	Pulse pulse(500);
	Debounce sw(SW);
	while(1) {
		if(sw.falling()) {
			puts("Rotary button pushed");
		}
		if(pulse.expired())
			pi_gpio_toggle(LED);
	}

	return 0;
}

