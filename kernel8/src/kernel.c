#include "mini_uart.h"
#include "delays.h"
#include "gpio.h"

#define uart_puts uart_send_string

void test_delay()
{
	uart_puts("Waiting 1000000 CPU cycles (ARM CPU): ");
	wait_cycles(1000000);
	uart_puts("OK\r\n");

	uart_puts("Waiting 1000000 microsec (ARM CPU): ");
	wait_msec(1000000);
	uart_puts("OK\r\n");

	uart_puts("Waiting 1000000 microsec (BCM System Timer): ");
	if(get_system_timer()==0) {
		uart_puts("Not available\r\n");
	} else {
		wait_msec_st(1000000);
		uart_puts("OK\r\n");
	}
}

void kernel_main(void)
{
	uart_init(9600);
	uart_send_string("Hello, world!\r\n");

	//test_delay();

	// set BCM23 to output (physical pin 16)
	GPFSEL2 &= ~(0b111<<9);
	GPFSEL2 |= (0b001<<9);
	GPSET0 |= (1<<23); // turn it on
	wait_msec_st(2000000);
	GPCLR0 |= (1<<23); // turn it off


	uart_send_string("I will now echo what you type\r\n");
	while (1) {
		char c = uart_recv();
		uart_send(c);
		if(c=='\r') uart_send('\n'); // prolly needs linefeed
	}
}
