#define F_CPU 1000000UL


#include <avr/io.h>
#include <util/delay.h>

typedef uint8_t u8;

typedef struct {
	u8 ipin; // input pin
	u8 opin; // output pin
	u8 grate; // "integrater" value
} bounce_t;


bounce_t bounces[]= {
	bounce_t{1<<PB3, 1<<PB2, 0xFF},
	bounce_t{1<<PB4, 1<<PB1, 0xFF},
	0
};


int main()
{
	// initialisation of pins
	bounce_t *b = bounces;
	while(b->ipin) {
		DDRB  |= b->opin; // set the output pin to output
		PORTB |= b->opin; // set the output pin high
		PORTB |= b->ipin; // activate pull-up resistor on input pin
		b++; // next set of pins
	}

	for(;;) {
		b = bounces;
		while(b->ipin) {
			bool on = b->ipin & PINB;
			b->grate = (b->grate <<1) | on; // update the integrator 

			if(b->grate == 0xFF) PORTB |= b->opin;
			if(b->grate == 0x00) PORTB &= ~b->opin;
			b++;
		}
		_delay_ms(3);
	}
}
