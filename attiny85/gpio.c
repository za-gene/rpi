#include <avr/io.h>
//#include <avr/iotnx5.h> // io specific to t85

#include "gpio.h"

void digitalWrite(uint8_t pin, int value)
{
	if(value == 0)
		PORTB &= ~(1<<pin);
	else
		PORTB |= (1<<pin);
}

void pinMode(uint8_t pin, uint8_t mode)
{
	switch(mode) {
		case OUTPUT:
			DDRB |= (1 << pin);
			break;
		case INPUT_PULLUP:
			PORTB |= (1<<pin);
			DDRB &= ~(1 << pin);
			break;
		case INPUT:
			DDRB &= ~(1 << pin);
			break;
	}
			
}

int digitalRead(uint8_t pin)
{
	if(PINB & (1<<pin))
		return 1;
	else
		return 0;

}


void gpio_toggle(uint8_t pin)
{
	PORTB ^= (1<<pin);
}
