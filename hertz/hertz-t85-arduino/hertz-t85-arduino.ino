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

#define E 0b1011 // letter E for display
#define H 0b1100 // letter H for display
#define L 0b1101 // letter L for display
#define P 0b1110 // letter P for display

void spi_write(uint8_t value)
{
	//while(digitalRead(SCK) == HIGH);
	//digitalWrite(MOSI, value >> 7);
	//value <<=1;
	uint8_t recd = 0;
	for(uint8_t i = 0; i<8; i++) {
		digitalWrite(MOSI, value >> 7);
		value <<= 1;
		digitalWrite(SCK, HIGH);
		nops(1);
		digitalWrite(SCK, LOW);
	}
}

void transfer_7219(uint8_t address, uint8_t value) {
	digitalWrite(CS, LOW); 
	//shiftOut(MOSI, SCK, MSBFIRST, address);
	//shiftOut(MOSI, SCK, MSBFIRST, value);
	spi_write(address);
	spi_write(value);
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


void display_count(u8 mode, u32 cnt) {
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

	transfer_7219(8, mode);
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
volatile u32 g_total_num_falls = 0; /// never reset

ISR(INT0_vect)
{
	g_num_falls++;
	g_total_num_falls++;
}

/////////////////////////////////////////////////////////////////////////
// TIMER1


//#define LED  PB4


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

const u16 timer1_freq = 1000;
const u32 fudged_freq = (u32)timer1_freq *10170/10000; // improve the accuracy

volatile u8 timer1_triggered =1;
volatile u32 timer1_hz_count = 0;
ISR(TIMER1_COMPA_vect)
{
	// remember: interrupts aren't nested
	static volatile u16 cnt = 0;
	if(++cnt != fudged_freq) return;
	cnt = 0;

	timer1_triggered = 1;
	timer1_hz_count = g_num_falls;
	g_num_falls = 0;

}


/////////////////////////////////////////////////////////////////////////
// DEBOUNCE

#define BTN PB4

//uint8_t intergrator = 0xFF;

void button_init()
{
	pinMode(BTN, INPUT_PULLUP);
}

bool button_fell = false;

void button_poll()
{
#define MAX_COUNT 1000UL	
	static u32 count = 0;
	if(digitalRead(BTN) == HIGH) {
		count = 0;
	} else {
		count ++;
		if(count == MAX_COUNT) button_fell = true;
		if(count > MAX_COUNT) count = MAX_COUNT+1;
	}
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
	init_timer1(timer1_freq); // use 1000Hz because there is a minumum
	sei();
	//pinMode(LED, OUTPUT);
	button_init();

}

enum state_e { freq_st = 0, count_st };

void loop() {
	static uint8_t state = freq_st;

	bool refresh_display = false;
	if(timer1_triggered) {
		timer1_triggered = 0;
		refresh_display = true;
		//display_count(H, timer1_hz_count);
	}

	button_poll();
	//static int temp = 0;
	if(button_fell) {
		button_fell = false;
		refresh_display = true;
		state++;
		if(state>count_st) state = freq_st;
		//display_count(L, ++temp);
	}

	static uint32_t prev_num_falls = 0;
	if((prev_num_falls != g_total_num_falls) && state == count_st) {
		refresh_display = true;
		prev_num_falls = g_total_num_falls;
	}

	if(!refresh_display) return;
	switch(state) {
		case freq_st: 	
			display_count(H, timer1_hz_count); 
			break;
		case count_st:
			display_count(L, g_total_num_falls); 
			break;
		default:
			display_count(E, 666);
	}
	//display_count(L, g_total_num_falls);
	//display_count(E, millis());
}

