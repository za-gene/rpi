#define F_CPU 8000000UL
#include "minitone85.h"

#include <avr/io.h>
#include <avr/iotnx5.h> // for CLKPR
#include <util/delay.h>




int main()
{
	//CLKPR = 0x80;
	//CLKPR = 0;
	DDRB = 1<<PB1; // set PB1 for output
	PORTB = 0x00;  // set all pins low
	miniTone(440); 

	for(;;); // forever
}
