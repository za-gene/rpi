/** MAX7129 - zeroseg
 * Connection:
 * 
 *
 *  | MAX7219/MAX7221 | PI    |
 *  | --------------- | ----- |
 *  | VCC             | 5V    |
 *  | DIN             | 10/19 |
 *  | CLK             | 11/23 |
 *  | LOAD/CS         | 8/24  |
 *  Pins are stated as GPIO/PHYSICAL
 */

#include <basal.h>
#include <bcm2835.h>

//#define LOAD_PIN 5

/**
  Transfers data to a MAX7219/MAX7221 register.

  @param address The register to load data into
  @param value   Value to store in the register
  */
void transfer_7219(u8 address, u8 value) 
{
	/*
	digitalWrite(LOAD_PIN, LOW); // CS
	bcm2835_spi_transfer(address);
	bcm2835_spi_transfer(value);
	digitalWrite(LOAD_PIN, HIGH);
	*/

	u16 val = address;
	val = (val<<8) + value;
	bcm2835_spi_write(val);

}

void init_7219() 
{
	//pinMode(LOAD_PIN, OUTPUT);
	//SPI.setBitOrder(MSBFIRST);
	//SPI.begin();
	transfer_7219(0x0F, 0x00);
	transfer_7219(0x09, 0xFF); // Enable mode B
	transfer_7219(0x0A, 0x0F); // set intensity (page 9)
	transfer_7219(0x0B, 0x07); // use all pins
	transfer_7219(0x0C, 0x01); // Turn on chip
}


