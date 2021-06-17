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

#include "pi.h"

#define BTN  22 // GPIO number, not physical pin
#define LED  25 // GPIO of built-in LED

volatile uint32_t count = 0;

// NB as of 2021-06-21 the GPIO parameter is ignored, and is enabled on any pin. According to SDK
 
void gpio_callback(uint gpio, uint32_t events)
{
	count++;
	pi_gpio_toggle(LED);
}

int main() 
{
	stdio_init_all();
	// while(!tud_cdc_connected()) sleep_ms(250); // wait for usb serial 

	pi_max7219_init();
	pi_max7219_show_count(count);

	pi_gpio_init(LED, OUTPUT);
	pi_gpio_init(BTN, INPUT_PULLUP);
	gpio_set_irq_enabled_with_callback(BTN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

	while(1) {
		pi_max7219_show_count(count);
		busy_wait_us_32(100*1000);
	}

	return 0;
}

