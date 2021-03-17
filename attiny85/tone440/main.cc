#define F_CPU 8000000UL
#include "minitone85.h"

#include <avr/io.h>
#include <avr/iotnx5.h> // for CLKPR
#include <util/delay.h>
#include <stdlib.h>


// noise pin seems to interfere with output, so disable it
//
#define NOISE_PIN (1<<PB0)

int main()
{
	//CLKPR = 0x80;
	//CLKPR = 0;
	DDRB = 1<<PB1; // set PB1 for output
	PORTB = 0x00;  // set all pins low
	miniTone(440); 

	// white noise on pin PB4 (physical pin 3)
	DDRB |= NOISE_PIN;
	for(;;) {
		continue;
		if((random() % 2) == 0)
			PORTB |= NOISE_PIN;
		else
			PORTB &= ~NOISE_PIN;
	}
}
