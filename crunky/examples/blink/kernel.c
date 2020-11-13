#include <delays.h>
#include <gpio.h>
#include <mini_uart.h>


void kernel_main(void)
{
	uart_init(9600);
	uart_puts("Hello from from blinky sketch\r\n");
	const int bcm_pin = 20;
	gpio_sel(bcm_pin, OUTPUT); // set its pin mode to OUTPUT
	while(1) {
		gpio_set(bcm_pin); // set the pin high
		delay_ms(500);  // wait 0.5 secs
		gpio_clr(bcm_pin); // turn the pin off
		delay_ms(1000); // wait 0.5 secs
	}
}
