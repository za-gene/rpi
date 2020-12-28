#define F_CPU 1000000UL


#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>

typedef uint8_t u8;

void debounce(u8 input, u8 output, u8* grate)
{
	*grate <<= 1; //shift the integrator
	*grate |= (bit_is_set(PINB, input) != 0); // update it
	if(*grate == 0xFF) PORTB |= _BV(output);
	if(*grate == 0x00) PORTB &= ~_BV(output);
}


int main()
{
	// PB:    43210 pin config (below)
	DDRB  = 0b00110; // set PB1,2 as outputs
	PORTB = 0b11110; // set inputs as pullups; outputs high

	u8 grate1 = 0xFF, grate2 = 0xFF; // init integrators for outputs

loop:
	debounce(PB3, PB2, &grate2);
	debounce(PB4, PB1, &grate1);
	_delay_ms(3);
	goto loop;

}
