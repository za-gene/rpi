#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/spi.h"
#include "hardware/gpio.h"


//#define BTN 14 // GPIO number, not physical pin
#define LED 25 // GPIO of built-in LED

#define NUM_PINS 30 // GPIO pins are 0-29 inc.. There are no GPIO30 or GPIO31

#define FOR_PINS for(int i = 0; i< NUM_PINS; i++)

static uint8_t integrators[NUM_PINS];
static uint32_t gpio_mask = 0; // the pins we are interested in debouncing
static uint32_t falling_mask = 0;
static uint32_t rising_mask = 0;

static bool repeating_timer_callback(struct repeating_timer *t)
{
	uint32_t mask = gpio_mask;
	uint32_t values = gpio_get_all();
	FOR_PINS {
		if(mask & 1) {
			int old_val = integrators[i];
			integrators[i] <<= 1;
			integrators[i] |= (values & 1);
			if(old_val != 0xFF && integrators[i] == 0xFF) {
		//gpio_put(LED, 1);
				rising_mask |= (1<<i);
				//falling_mask |= (1<<i);
			}
			if(old_val != 0 && integrators[i] == 0) {
		//gpio_put(LED, 1);
				falling_mask |= (1<<i);
			}

		}
		mask >>= 1;
		values >>= 1;
	}

	return true;
}

static struct repeating_timer timer;

void debounce_gpio(uint gpio)
{
	static bool initialised = false;
	if(!initialised) {
		initialised = true;
		add_repeating_timer_ms(3, repeating_timer_callback, NULL, &timer);
		FOR_PINS integrators[i] = 0xFF;
	}

	gpio_init(gpio);
	gpio_set_dir(gpio, GPIO_IN);
	gpio_pull_up(gpio);
	gpio_mask |= (1<<gpio);
}

bool debounce_falling(uint gpio)
{
	bool yes = falling_mask & (1<<gpio);
	falling_mask &= ~(1<<gpio);
	return yes;
}

bool debounce_rising(uint gpio)
{
	bool yes = rising_mask & (1<<gpio);
	rising_mask &= ~(1<<gpio);
	return yes;
}

int main() 
{
	stdio_init_all();

	const uint sw = 16;
	//Debounce button(16);
	debounce_gpio(sw);

	gpio_init(LED);
	gpio_set_dir(LED, GPIO_OUT);

	for(;;) {
		if(debounce_falling(sw)) {
		gpio_put(LED, 1);

		//	gpio_xor_mask(1<<LED); // toggle LED

		}
	}

	return 0;
}

