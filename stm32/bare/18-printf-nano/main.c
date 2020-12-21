#include <stdio.h>
#include <stdlib.h>

#include <gpio.h>
#include <usart.h>

void main()
{
	init_serial();
	beep();
	gpio_mode(BUILTIN_LED, OUTPUT);
	
	printf("\nsprintf test\n");
	int i = 0;
	while(1) {
		printf("Loop number %d\n", i++);
		gpio_toggle(BUILTIN_LED);
		delayish(500);
	}
}
