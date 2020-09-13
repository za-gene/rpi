#include <string.h>
#include <stddef.h> // for size_t
#include <stdio.h>
#include "../blue.h"


char greeting[] = "Hello from bare metal usart 8";


void main() 
{
	init_serial();
	putchar('\a'); // beep
	puts(greeting);
	puts("You type, I echo.");
	while(1) {
		int c = getchar();
		putchar(c);
	}
}
