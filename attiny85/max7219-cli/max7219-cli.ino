#define F_CPU 1000000UL


#include <tinySPI.h>

#define CS PB4

/**
  Transfers data to a MAX7219/MAX7221 register.
  @param address The register to load data into
  @param value   Value to store in the register
 */
void transfer_7219(uint8_t address, uint8_t value) {
	digitalWrite(CS, LOW);
	SPI.transfer(address);
	SPI.transfer(value);
	digitalWrite(CS, HIGH);
}


void setup() {
	pinMode(CS, OUTPUT);
	SPI.begin();
	transfer_7219(0x0F, 0x00);
	transfer_7219(0x09, 0xFF); // Enable mode B
	transfer_7219(0x0A, 0x0F); // set intensity (page 9)
	transfer_7219(0x0B, 0x07); // use all pins
	transfer_7219(0x0C, 0x01); // Turn on chip
}

void loop() {
	static int cnt = 0;
	int num = cnt++;
	for (uint8_t i = 0; i < 8; ++i)
	{
		int val = num %  10;
		transfer_7219(8 - i, val);
		num = num/10;
		delay(1);
	}
	delay(1000);
}

