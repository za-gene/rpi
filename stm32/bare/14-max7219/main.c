#include <gpio.h>
#include <spi.h>

#define CS PA4 // SPI chip select, aka max7219 "load pin"

void transfer_7219(uint8_t address, uint8_t value) 
{
	gpio_write(CS, 0); 
	spi_transfer(address);
	spi_transfer(value);
	gpio_write(CS, 1);
}

void init_7219() 
{
	transfer_7219(0x0F, 0x00);
	transfer_7219(0x09, 0xFF); // Enable mode B
	transfer_7219(0x0A, 0x0F); // set intensity (page 9)
	transfer_7219(0x0B, 0x07); // use all pins
	transfer_7219(0x0C, 0x01); // Turn on chip
}



void main()
{
	gpio_mode_out(CS);
	gpio_write(CS, 1);
	init_spi();
	init_7219();

	u16 val = 0; // unsigned will give strange results if negative
	while (1)
	{
		u16 tmp = val++;
		for(u8 i=1; i<=8; i++) {
			u16 digit = tmp %10;
			tmp = tmp / 10;
			transfer_7219(i,digit);
		}
		delayish(200);
	}
}

