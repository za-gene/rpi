#define F_CPU 8000000

//#include <tinySPI.h> // doesn't seem to work

/* use usual pins for SPI:
   PB2 SCK
   PB0 MOSI
   MISO not required
   Use 5V input to the display
 */

#define CS PB1
#define MOSI PB0
#define SCK PB2 // traditionally reserved for MISO, but we don't need it

typedef uint32_t u32; 

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

void setup() {
	CLKPR = 1<< CLKPCE; // tell the chip we're goint ot scale it
	CLKPR = 0; // to 8MHz
	init_7219();

}

void loop() {
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
