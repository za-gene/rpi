#include <mini_uart.h>
//#include <delays.h>
//#include <gpio.h>

//#define uart_puts uart_send_string


void kernel_main(void)
{
	uart_init(9600);
	uart_puts("uart test\r\n");
	uart_puts("I will now echo what you type\r\n");
	while (1) {
		char c = uart_recv();
		uart_send(c);
		if(c=='\r') uart_send('\n'); // prolly needs linefeed
	}
}
