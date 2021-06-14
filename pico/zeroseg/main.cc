#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/adc.h"
//#include "hardware/clocks.h"
//#include "hardware/flash.h"
#include "hardware/gpio.h"
#include "hardware/time.h"
//#include "hardware/irq.h"
//#include "hardware/pwm.h"
//#include "hardware/spi.h"
// #include "tusb.h" // if you want to use tud_cdc_connected()

#include "pi.h"

#define BTN  14 // GPIO number, not physical pin
#define LED  25 // GPIO of built-in LED


void gpio_callback(uint gpio, uint32_t events)
{
	pi_gpio_toggle(LED);
}

int main() 
{
	//stdio_init_all();
	// while(!tud_cdc_connected()) sleep_ms(250); // wait for usb serial 

	pi_max7219_init();

	uint32_t count = 0;
	while(1) {
		pi_max7219_show_count(count++);
		sleep_ms(1000);
	}

	pi_gpio_init(LED, OUTPUT);
	pi_gpio_init(BTN, INPUT); // have it as floating
	gpio_set_irq_enabled_with_callback(BTN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

	while(1);

	return 0;
}

