//#include <string.h>
//#include <stddef.h> // for size_t
//#include <stdio.h>
//#include <blue.h>
#include <usart.h>



void main() 
{
char greeting[] = "Hello from bare metal usart 8";
	init_serial();
	putchar('\a'); // beep
	puts(greeting);
	puts("You type, I echo.");
	while(1) {
		int c = getchar();
		putchar(c);
	}
}
