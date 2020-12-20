#include <stdio.h>
#include <stdlib.h>

#include <gpio.h>
#include <usart.h>




char str[100];


void main()
{
	init_serial();
	beep();
	gpio_mode(BUILTIN_LED, OUTPUT);
	
	puts("\nsprintf test 5");
	/*
	sprintf(str, "_sheap:%x", &_sheap);
	puts(str);
	sprintf(str, "_eheap:%x", &_eheap);
	puts(str);
	*/

	sprintf(str, "0x20000000 %x", 0x20000000);
	puts(str);

	puts("A quick loop 5 times");
	for(int i=0; i<5; ++i) {
		sprintf(str, "Loop number %d", i);
		puts(str);
		gpio_write(BUILTIN_LED, 1);
		delayish(500);
		gpio_write(BUILTIN_LED, 0);
		delayish(500);
	}

	puts("Now let's try to malloc()");
	int size = 10;
	while(1) {
		sprintf(str, "Trying to malloc of size %d", size);
		puts(str);
		void* mem = malloc(size);
		if(mem ==0) {
			puts("Panic: no more memory. Hanging.");
			while(1);
		}
		size *= 2;
	}


}

