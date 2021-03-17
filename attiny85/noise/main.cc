#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/iotnx5.h> // for CLKPR
#include <util/delay.h>
#include <stdlib.h>


#define NOISE_PIN (1<<PB1)

int main()
{
	DDRB |= NOISE_PIN; // for output
	for(;;) {
		continue;
		if((random() % 2) == 0)
			PORTB |= NOISE_PIN;
		else
			PORTB &= ~NOISE_PIN;
	}
}
