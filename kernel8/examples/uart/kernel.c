#include <mini_uart.h>
#include <delays.h>
#include <gpio.h>

#define uart_puts uart_send_string


void kernel_main(void)
{
	uart_init(9600);
	uart_send_string("Hello, world!\r\n");


	uart_send_string("I will now echo what you type\r\n");
	while (1) {
		char c = uart_recv();
		uart_send(c);
		if(c=='\r') uart_send('\n'); // prolly needs linefeed
	}
}
