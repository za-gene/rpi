#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>


#define LED 1<<PB4 // physical pin 3

int main()
{
	DDRB = LED; // set PB4 for output
	PORTB = 0x00;  // set all pins low

	for(;;) {
		PORTB |= LED;
		_delay_ms(100);
		PORTB &= ~LED;
		_delay_ms(1000);
	}
}
