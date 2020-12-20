#include <stdio.h>
#include <gpio.h>


//#define LED PB13

//void delay() { delaysecsish(1); }


void main()
{

	gpio_mode(BUILTIN_LED, OUTPUT);


	while (1) {
		gpio_write(BUILTIN_LED, 1);
		delayish(1000);
		gpio_write(BUILTIN_LED, 0);
		delayish(1000);
	}

}

