/* Pin toggling via interrupts
Put a button on PB2
and an LED on PB4
*/

#include "avr/interrupt.h"


#define LED PB4

void setup()
{
	// set up PB2 to detect falling edges
	GIMSK |= (1<<6);	// INT0 bit
	MCUCR |= 0b10;		// falling edge
	sei();                 // enables interrupts

	pinMode(LED, OUTPUT);
}

void loop()
{
}

ISR(INT0_vect)
{
	static volatile int on = false;
	on = 1 - on;	
	digitalWrite(LED, on);
}

