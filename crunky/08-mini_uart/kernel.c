#include <mini_uart.h>
#include <stdio.h>

void kernel_main(void)
{
	//uart_init(9600);
	mini_uart_init_as_stdio();
	puts("uart test");
	puts("I will now echo what you type");
	while (1) {
		char c = getchar();
		printf("%c %d\n", c, c);
		//putchar(c);
		//if(c=='\r') uart_send('\n'); // prolly needs linefeed
	}
}
