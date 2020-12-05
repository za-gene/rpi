#include <mini_uart.h>
#include <stdio.h>

void kernel_main(void)
{
	//uart_init(9600);
	uart_init_as_stdio(115200);
	puts("uart test");
	puts("I will now echo what you type");
	while (1) {
		char c = getchar();
		putchar(c);
		//if(c=='\r') uart_send('\n'); // prolly needs linefeed
	}
}
