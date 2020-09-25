#include <stm8.h>
#include <millis.h>

#define CS PA3 // SPI chip select

void main()
{
	gpio_mode_out(PD4);
	gpio_mode_out(PD5);
	gpio_mode_out(PD6);
	gpio_mode_out(CS); // apparently needed
	gpio_mode_out(BUILTIN_LED);
	//gpio_write(CS, 1);
	init_millis();

	// set speed to 2MHz/8 = 250,000
	SPI_CR1 |= (0b010 << 3); // 0b010 is bause rate f_MASTER/8, 3 is baud rate bits
	SPI_CR1 |= SPI_CR1_MSTR; // master configuration
	SPI_CR1 |= SPI_CR1_SPE; // enable peripheral

	u8 val = 0;
	SPI_DR = val; // 
	while (1)
	{
		//gpio_write(CS, 0);
		while(!(SPI_SR & SPI_SR_RXNE));
		//while((SPI_SR & SPI_SR_TXE));
		val = SPI_DR;
		SPI_DR = 0; // seems necessary
		//gpio_write(CS, 1);

		gpio_write(PD6, val & 0b1);
		gpio_write(PD5, val & 0b10);
		gpio_write(PD4, val & 0b100);
		
		gpio_toggle(BUILTIN_LED);
		//++val;
		delay_millis(1000);
	}
}

