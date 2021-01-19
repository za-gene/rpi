#define F_CPU 1000000UL


#include <avr/io.h>
//#include <avr/sfr_defs.h>
#include <util/delay.h>
#include <avr/interrupt.h>

typedef uint8_t u8;

#define OUTPUT		1
#define INPUT		2
#define INPUT_PULLUP	3

#define LOW	0
#define HIGH	1

void pinMode(u8 pb, u8 mode)
{
	switch(mode) {
		case OUTPUT:
			DDRB |= (1<<pb);
			break;
		case INPUT:
			DDRB &= ~(1<<pb);
			break;
		case INPUT_PULLUP:
			DDRB &= ~(1<<pb);
			PORTB |= (1<<pb);
			break;
	}
}

u8 digitalRead(u8 pb)
{
	return bit_is_set(PINB, pb)? 1 : 0;
}

void digitalWrite(u8 pb, u8 value)
{
	if(value)
		PORTB |= _BV(pb);
	else
		PORTB &= ~_BV(pb);
}

void digitalToggle(u8 pb)
{
	digitalWrite(pb, 1 - digitalRead(pb));
}


bool high(uint8_t pin) { return PINB & (1<<pin) ; }
bool low(uint8_t pin)  { return high(pin) != 0; }

volatile u8 count = 0;

#define GIMSK_INT0 (1<<6)
#define GIMSK_PCIE (1<<5)

static volatile u8 old_pb2;
static volatile u8 old_pb4;

#define CS 	PB4
#define SCK 	PB2
#define MISO 	PB1

//static void
//__attribute__((interrupt(2)))
ISR(INT0_vect)
{
	//digitalWrite(PB1, digitalRead(PB2));
	digitalWrite(MISO, count & (1<<7));
	if(digitalRead(CS) == LOW)
		count <<= 1;

}

ISR(PCINT0_vect)
{
#if 1
	//digitalWrite(PB3, digitalRead(PB4));
	if(digitalRead(CS)) count++;
#else
	volatile u8 pb2 = digitalRead(PB2);
	if(old_pb2 != pb2) {
		old_pb2 = pb2;
		digitalWrite(PB1, pb2);
	}

	volatile u8 pb4 = digitalRead(PB4);
	if(old_pb4 != pb4) {
		old_pb4 = pb4;
		digitalWrite(PB3, pb4);
	}
#endif
}

#define MCUCR_ISC01 (1<<1)

int main()
{
	CLKPR = 1 << CLKPCE; // Tell chip we want to scale the clock
	CLKPR = 0; // prescaler is 1, giving 8MHz

	u8 value = 0, bits;
	pinMode(MISO, OUTPUT);
	pinMode(SCK, INPUT);
	//MCUCR |= (1<<MCUCR_ISC01);

	pinMode(CS, INPUT);

	//PCMSK |= _BV(CS); // Pin Change Mask. Handle changes on PB2 and PB4
	//GIMSK |= GIMSK_INT0 | GIMSK_PCIE; // Enable pin change interrupt in Gen Interrupt Mask
	//sei(); // enable interrupts

	//pinMode(PB0, OUTPUT);
send_byte:
	bits = value;
	while(high(CS));
	//while(low(SCK));
	//for(uint8_t i = 0; i< 8; i++) {
	while(low(CS)) {
		digitalWrite(MISO, bits>>7);
		while(high(SCK));
		bits <<= 1;
		//digitalWrite(MISO, bits>>7);
		while(low(SCK));
	}
	value++;
	//while(low(CS));
	goto send_byte;
}
