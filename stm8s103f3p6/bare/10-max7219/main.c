#include <stm8.h>
#include <millis.h>

#define CS PA3 // SPI chip select, aka max7219 "load pin"

void spi_transfer(u8 value)
{
	SPI_DR = value;
	while(!(SPI_SR & SPI_SR_RXNE));
	(void) SPI_DR;
}


void transfer_7219(uint8_t address, uint8_t value) {
	gpio_write(CS, 0); 
	spi_transfer(address);
	spi_transfer(value);
	gpio_write(CS, 1);
}

void init_7219() {
	//pinMode(LOAD_PIN, OUTPUT);
	
	//SPI.setBitOrder(MSBFIRST);
	//SPI.begin();
	transfer_7219(0x0F, 0x00);
	transfer_7219(0x09, 0xFF); // Enable mode B
	transfer_7219(0x0A, 0x0F); // set intensity (page 9)
	transfer_7219(0x0B, 0x07); // use all pins
	transfer_7219(0x0C, 0x01); // Turn on chip
}

void init_spi()
{
	// set speed to 2MHz/8 = 250,000
	//SPI_CR1 |= (0b010 << 3); // 0b010 is bause rate f_MASTER/8, 3 is baud rate bits
	SPI_CR1 |= SPI_CR1_MSTR; // master configuration
	SPI_CR1 |= SPI_CR1_SPE; // enable peripheral
}


void main()
{
	gpio_mode_out(CS);
	gpio_write(CS, 1);
	init_spi();
	init_7219();
	init_millis();
	//gpio_mode_out(BUILTIN_LED);

	u16 val = 0; // unsigned will give strange results if negative
	while (1)
	{
		u16 tmp = val++;
		for(u8 i=1; i<=8; i++) {
			u16 digit = tmp %10;
			tmp = tmp / 10;
			transfer_7219(i,digit);
		}

		//gpio_toggle(BUILTIN_LED);
		//delay_millis(10);
	}
}

