#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/iotnx5.h> // for CLKPR
//#include <util/delay.h>
#include <stdlib.h>
//#include <avr/power.h>


#define NOISE_PIN (1<<PB1)

int main()
{
	// set clock to 8Mhz
	CLKPR = 1 << CLKPCE; // Tell chip we want to scale the clock
	CLKPR = 0; // prescaler is 1, giving 8MHz

	DDRB |= NOISE_PIN; // for output
	for(;;) {
		if((random() % 2) == 0)
			PORTB |= NOISE_PIN;
		else
			PORTB &= ~NOISE_PIN;
	}
}
