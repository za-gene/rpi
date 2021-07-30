#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/adc.h"
//#include "hardware/clocks.h"
//#include "hardware/flash.h"
#include "hardware/gpio.h"
//#include "hardware/irq.h"
//#include "hardware/pwm.h"
#include "ssd1306.h"
// #include "tusb.h" // if you want to use tud_cdc_connected()



int main() 
{
	stdio_init_all();
	// while(!tud_cdc_connected()) sleep_ms(250); // wait for usb serial 

#define BTN  14 // GPIO number, not physical pin
#define LED  25 // GPIO of built-in LED
	gpio_init(BTN);
	gpio_set_dir(BTN, GPIO_IN);
	gpio_pull_up(BTN);
	// gpio_get() gets state of pin

	gpio_init(LED);
	gpio_set_dir(LED, GPIO_OUT);
	init_display(64, 4);

	ssd1306_print("transfer prog\n");
	show_scr();
	int i = 0;
	for(;;) {
		char c = getchar();
		if(c != 'T') continue;
		uint32_t size = 0;
		for(int i = 0;  i<4; i++) {
			c = getchar();
			size += (c << (i*8));
		}

		char msg[80];
		sprintf(msg, "size %d\n", size);
		ssd1306_print(msg);
		show_scr();
		/*
		gpio_put(LED, 1);
		sleep_ms(100);
		gpio_put(LED, 0);
		sleep_ms(1000);	
		*/
	}

	return 0;
}

