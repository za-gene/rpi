#include <uart.h>
#include <util/delay.h>


int main()
{
	uart_init(115200);
	while(1) {
		uart_puts("hello");
		_delay_ms(1000);
	}
}

