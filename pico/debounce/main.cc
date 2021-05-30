//#include <stdio.h>
//#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/spi.h"
#include "hardware/gpio.h"

#include "debounce.h"


//#define BTN 14 // GPIO number, not physical pin
#define LED 25 // GPIO of built-in LED

void toggle()
{
	gpio_xor_mask(1<<LED); // toggle LED
}

int main() 
{
	stdio_init_all();

	Debounce button(20);

	gpio_init(LED);
	gpio_set_dir(LED, GPIO_OUT);

	for(;;) {
		if(button.falling()) toggle();
		//if(button.rising()) toggle();
	}

	return 0;
}

