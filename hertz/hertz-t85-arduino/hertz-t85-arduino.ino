// vim: ft=cpp

#define F_CPU 8000000

#include <avr/io.h>
#include <avr/interrupt.h>


typedef uint8_t u8; 
//typedef uint16_t u16; 
typedef uint32_t u32; 


/////////////////////////////////////////////////////////////////////////
// DISPLAY


// We use completely non-standard pins
// We are bit-banging anyway
#define CS 	PB0
#define MOSI 	PB1
#define SCK 	PB3


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


void nops(u32 n) 
{ 
	while(n--) { asm volatile ("nop"); }
}


void display_count(u32 cnt) {
	//static u32 cnt = 0; // "int" is too limiting
	static u8 heart_beat = 0;
	u32 num = cnt;
	for (uint8_t i = 0; i < 8; ++i)
	{
		u8 c = num % 10;
		num /= 10;
		u8 sep = 0; // thousands separator

		// add in thousands separators
		if((i>0) && (i % 3 == 0)) {sep = 1<<7; }

		// maybe put int a heartbeat
		if(i==0) {
			heart_beat = 1 - heart_beat;
			if(heart_beat) {sep = 1<<7; }
		}

		// blank if end of number
		if((c==0) && (num==0) && (i>0)) { sep = 0; c = 0b1111; }

		c |= sep;

		transfer_7219(i+1, c);
		nops(1000);
	}
	//cnt++;
	//delay(10);

}

/////////////////////////////////////////////////////////////////////////
// FALLING



void init_falling()
{
	// set up PB2 to detect falling edges
	GIMSK |= (1<<6);	// INT0 bit
	MCUCR |= 0b10;		// falling edge
}

volatile u32 g_num_falls = 0;

ISR(INT0_vect)
{
	g_num_falls++;
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

volatile u8 timer1_triggered =1;
volatile u32 timer1_hz_count = 0;
ISR(TIMER1_COMPA_vect)
{
	// remember: interrupts aren't nested
	static volatile u16 cnt = 0;
	if(++cnt != 1000) return;
	//cnt++;
	//if(++cnt % 1000 != 0) return;
	cnt = 0;

	timer1_triggered = 1;
	timer1_hz_count = g_num_falls;
	g_num_falls = 0;

}


/////////////////////////////////////////////////////////////////////////
// ENSEMBLE

void setup() {
#if F_CPU == 8000000
	CLKPR = 1<< CLKPCE; // tell the chip we're goint ot scale it
	CLKPR = 0; // to 8MHz
#endif

	// scrap any timers that might have been set up
	TIMSK = 0; // Timer/Counter Interrupt Mask Register

	init_7219();
	init_falling();
	init_timer1(1000); // use 1000Hz because there is a minumum
	sei();
	pinMode(LED, OUTPUT);
}

void loop() {
	if(timer1_triggered) {
		timer1_triggered = 0;
		//static volatile u32 displayx = 0;
		display_count(timer1_hz_count);
		digitalWrite(LED, 1 - digitalRead(LED));
	}
}

