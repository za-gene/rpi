#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>


#define LED (1<<PB4) // physical pin 3

int main()
{
	DDRB = LED; // set PB4 for output
	PORTB = 0x00;  // set all pins low

	// experiment to see how many amps a pin has
	DDRB  |= 1<<PB3; // physical pin 2 is output
	PORTB |= 1<<PB3; // set output high

	//while(1);

	for(;;) {
		PORTB |= LED;
		_delay_ms(100);
		PORTB &= ~LED;
		_delay_ms(1000);
	}
}
