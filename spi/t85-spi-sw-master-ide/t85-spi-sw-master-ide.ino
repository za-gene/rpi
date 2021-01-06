/*
 * A SPI master transmitter for the ATTiny85 using sw (software) bitbanging
 * 
 * 2021-01-06 created. Confirmed working
 */

#define CS    PB4
#define CLK   PB2
//#define MISO  PB1
#define MOSI  PB0 

typedef uint8_t u8;

void out(u8 pin, u8 init)
{
	pinMode(pin, OUTPUT);
	digitalWrite(pin, init);
}

void setup() {
	out(CS, HIGH);
	out(CLK, LOW);
	out(MOSI, LOW);
}

void pause(int n = 10) { delayMicroseconds(n); }

void mosi(u8* tfr, u8 pin)
{
	digitalWrite(pin, LOW);
	u8 hi = (*tfr) & (1<<7) ? 1 : 0;
	digitalWrite(MOSI, hi);
	pause();
	*tfr <<= 1;
}
	

void loop() {
	static u8 val = 0;
	u8 tfr = val;
	mosi(&tfr, CS);
	for(int i=0; i<8; i++) {
		digitalWrite(CLK, HIGH);
		pause();
		mosi(&tfr, CLK);
	}
	digitalWrite(CS, HIGH);
	pause();
	delay(1000);
	val++;
}
