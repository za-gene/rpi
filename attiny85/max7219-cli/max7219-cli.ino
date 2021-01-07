//#define F_CPU 1000000UL


#define CS    PB4
#define CLK   PB2
#define MOSI  PB0 

typedef uint8_t u8;
typedef uint32_t u32;

void out(u8 pin, u8 init)
{
	pinMode(pin, OUTPUT);
	digitalWrite(pin, init);
}

void setup_spi() {
	out(CS, HIGH);
	out(CLK, LOW);
	out(MOSI, LOW);
}

void pause(int n = 10) { delayMicroseconds(n); }

void mosi(u8* tfr, u8 pin)
{
	u8 hi = (*tfr) & (1<<7) ? 1 : 0;
	digitalWrite(MOSI, hi);
	pause();
	*tfr <<= 1;
}


void spi_transfer(u8 val) {
	u8 tfr = val;
	mosi(&tfr, CS);
	for(int i=0; i<8; i++) {
		digitalWrite(CLK, HIGH);
		pause();
		digitalWrite(CLK, LOW);
		mosi(&tfr, CLK);
	}
	pause();
}

/**
  Transfers data to a MAX7219/MAX7221 register.
  @param address The register to load data into
  @param value   Value to store in the register
 */
void transfer_7219(uint8_t address, uint8_t value) {
	digitalWrite(CS, LOW);
	spi_transfer(address);
	spi_transfer(value);
	digitalWrite(CS, HIGH);
}


void setup() {
	setup_spi();
	transfer_7219(0x0F, 0x00);
	transfer_7219(0x09, 0xFF); // Enable mode B
	transfer_7219(0x0A, 0x0F); // set intensity (page 9)
	transfer_7219(0x0B, 0x07); // use all pins
	transfer_7219(0x0C, 0x01); // Turn on chip
}

void loop() {
	static u32 cnt = 0;
	u32 num = cnt;
	for (uint8_t i = 0; i < 8; ++i)
	{
		u32 val = num %  10;
		//val = 7;
		transfer_7219(i+1, val);
		num = num/10;
		delay(1);
	}
	delay(1000);
	cnt++;
}

