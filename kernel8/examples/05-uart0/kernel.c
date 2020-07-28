//#include "delays.h"
//#include "gpio.h"
//#include "mini_uart.h"
#include "uart.h"

void kernel_main ( void )
{
	uart0_init();
	uart0_puts("uart0 test\r\n");
	while(1)
		uart0_send(uart0_getc());

}
