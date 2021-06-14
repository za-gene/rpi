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

#define BTN  14 // GPIO number, not physical pin
#define LED  25 // GPIO of built-in LED

enum pi_gpio_mode_e {INPUT, OUTPUT, INPUT_PULLUP};
void pi_gpio_init(uint gpio, pi_gpio_mode_e mode)
{
	gpio_init(gpio);
	switch(mode) {
		case INPUT_PULLUP:
			gpio_pull_up(gpio);
			[[fallthrough]];
		case INPUT:
			gpio_set_dir(gpio, GPIO_IN);
			break;
		case OUTPUT:
			gpio_set_dir(gpio, GPIO_OUT);
			break;
		default:
			assert(false);
	}
}

void pi_gpio_toggle(uint gpio)
{
	gpio_put(gpio, !gpio_get(gpio));
}

void gpio_callback(uint gpio, uint32_t events)
{
	pi_gpio_toggle(LED);
}

int main() 
{
	stdio_init_all();
	// while(!tud_cdc_connected()) sleep_ms(250); // wait for usb serial 

	pi_gpio_init(LED, OUTPUT);
	pi_gpio_init(BTN, INPUT); // have it as floating
	gpio_set_irq_enabled_with_callback(BTN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

	while(1);

	int i = 0;
	for(;;) {
		printf("Hello number %d\n", i++);
		gpio_put(LED, 1);
		sleep_ms(100);
		gpio_put(LED, 0);
		sleep_ms(1000);		
	}

	return 0;
}

