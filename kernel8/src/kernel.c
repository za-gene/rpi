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

#include <stddef.h>
#include "utils.h"
#define INPUT 0b000
#define OUTPUT 0b001

typedef unsigned int u32; // irrespective of architecture
typedef  unsigned long uintptr; // architecture-dependent

void gpio_sel(int bcm_pin, int mode)
{
	uintptr gpfsel = (bcm_pin/10)*4+ MMIO_BASE+ 0x200000;
	u32 value = get32(gpfsel);
	u32 shift = (bcm_pin%10)*3;
	value &= ~(0b111 << shift); // clear the bits
	value |= (mode << shift); // set to desired mode
	put32(gpfsel, value);

}
void gpio_clr(int bcm_pin)
{
	//volatile unsigned int* addr = (bcm_pin/32)*4 + MMIO_BASE + 0x200028;
	uintptr addr = (bcm_pin/32)*4 + MMIO_BASE + 0x200028;
	put32(addr, 1 << (bcm_pin%32));
}
void gpio_set(int bcm_pin)
{
	uintptr addr = (bcm_pin/32)*4 + MMIO_BASE + 0x20001C;
	put32(addr, 1 << (bcm_pin%32));
}
void delay_ms(int ms)
{
	for(int i=0; i<ms; ++i)
		wait_msec_st(1000);
}

void delay_s(int secs)
{
	for(int i=0; i<secs; ++i)
		delay_ms(1000);
}

void kernel_main(void)
{
	uart_init(9600);
	uart_send_string("Hello, world!\r\n");

	//test_delay();

	const int bcm_pin = 24;
	gpio_sel(bcm_pin, OUTPUT);
	while(1) {
		gpio_set(bcm_pin);
		delay_ms(500);
		gpio_clr(bcm_pin);
		delay_ms(1000);
	}


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
