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



int main() 
{
	stdio_init_all();
	// while(!tud_cdc_connected()) sleep_ms(250); // wait for usb serial 

	// dump out the second-stage bootloader
	const int wpl = 4; // words per line
	const int nrows = 256/4/wpl; // number of rows to output
	puts("\n.section .boot2, \"a\"");
	for(int r = 0; r< nrows; r++) {
		printf(".word ");
		for(int c = 0; c<wpl; c++) {
			printf("0x%8.8x", *(uint32_t*) (0x10000000 + (r*wpl+c)*4));
			if(c<(wpl -1)) printf(", ");
		}
		puts("");
	}

#define BTN  14 // GPIO number, not physical pin
#define LED  25 // GPIO of built-in LED
	gpio_init(BTN);
	gpio_set_dir(BTN, GPIO_IN);
	gpio_pull_up(BTN);
	// gpio_get() gets state of pin

	gpio_init(LED);
	gpio_set_dir(LED, GPIO_OUT);

	int i = 0;
	for(;;) {
		//printf("Hello number %d\n", i++);
		gpio_put(LED, 1);
		sleep_ms(100);
		gpio_put(LED, 0);
		sleep_ms(1000);		
	}

	return 0;
}

