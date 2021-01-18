#define F_CPU 1000000UL
#include <util/delay.h>
#include <stdlib.h>
#include <avr/io.h>
#include <uart1.h>

#define TX PB1

typedef unsigned int uint;

int main()
{
	init_uart1(F_CPU, TX);
	char str[10];
	uint i =0;
	for(;;) {
		utoa(i++, str, 10);
		char* s1= str;
		while(*s1) send_uart1(*s1++);
		send_uart1('\r');
		send_uart1('\n');
		_delay_ms(500);
	}
}
