// vim: ft=cpp

#define F_CPU 8000000

#include <avr/io.h>
#include <avr/interrupt.h>


typedef uint8_t u8; 
//typedef uint16_t u16; 
typedef uint32_t u32; 


/////////////////////////////////////////////////////////////////////////
// DISPLAY

#define CS PB1
#define MOSI PB0
#define SCK PB2 // traditionally reserved for MISO, but we don't need it


void transfer_7219(uint8_t address, uint8_t value) {
	digitalWrite(CS, LOW); 
	shiftOut(MOSI, SCK, MSBFIRST, address);
	shiftOut(MOSI, SCK, MSBFIRST, value);
	digitalWrite(CS, HIGH);
}

void init_7219() {
	pinMode(CS, OUTPUT);
	digitalWrite(CS, HIGH);
	pinMode(MOSI, OUTPUT);
	pinMode(SCK, OUTPUT);
	transfer_7219(0x0F, 0x00);
	transfer_7219(0x09, 0xFF); // Enable mode B
	transfer_7219(0x0A, 0x0F); // set intensity (page 9)
	transfer_7219(0x0B, 0x07); // use all pins
	transfer_7219(0x0C, 0x01); // Turn on chip
}

void init_display() {
	CLKPR = 1<< CLKPCE; // tell the chip we're goint ot scale it
	CLKPR = 0; // to 8MHz
	init_7219();

}

void display_count() {
	static u32 cnt = 0; // "int" is too limiting
	u32 num = cnt;
	for (uint8_t i = 0; i < 8; ++i)
	{
		u8 c = num % 10;
		num /= 10;
		u8 sep = 0; // thousands separator

		// add in thousands separators
		if((i>0) && (i % 3 == 0)) {sep = 1<<7; }

		// blank if end of number
		if((c==0) && (num==0)) { sep = 0; c = 0b1111; }

		c |= sep;

		transfer_7219(i+1, c);
		delay(1);
	}
	cnt++;
	//delay(10);

}

/////////////////////////////////////////////////////////////////////////
// FALLING

#include "avr/interrupt.h"


#define LED PB4

void init_falling()
{
	// set up PB2 to detect falling edges
	GIMSK |= (1<<6);	// INT0 bit
	MCUCR |= 0b10;		// falling edge
	sei();                 // enables interrupts

	pinMode(LED, OUTPUT);
}

ISR(INT0_vect)
{
	static volatile int on = false;
	on = 1 - on;	
	digitalWrite(LED, on);
}

/////////////////////////////////////////////////////////////////////////
// TIMER1


#define LED  PB4


void init_timer1(unsigned long freq)
{
	TCCR1 = 0; // stop the counter
	TCNT1 = 0;
	OCR1C = 0;
	if(freq==0) return;

	int prescale = 1;
	while(freq * 256 * (1<<(prescale-1)) < F_CPU)
		prescale++;
	TCCR1 = (1<<CTC1);
	TCCR1 |= prescale;
	int divisor = F_CPU/(1<<(prescale-1))/freq;
	OCR1C = divisor; // compare match value
	TIMSK |= (1<<OCIE1A); // enable compare match interrupt
	sei();
}

ISR(TIMER1_COMPA_vect)
{
	static volatile u16 cnt = 0;
	//cnt++;
	if(++cnt % 1000 != 0) return;
	cnt = 0;

	digitalWrite(LED, 1 - digitalRead(LED));
}


/////////////////////////////////////////////////////////////////////////
// ENSEMBLE

void setup() {
	init_display();
	init_falling();
	init_timer1(1000); // use 1000Hz because there is a minumum
	sei();
}

void loop() {
}

