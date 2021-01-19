#include <gpio.h>
#include <stm8.h>
#include <millis.h>

#define CS PA3 // SPI chip select

void main()
{
	pinMode(PD4, OUTPUT);
	pinMode(PD5, OUTPUT);
	pinMode(PD6, OUTPUT);
	pinMode(CS, OUTPUT); // apparently needed
	pinMode(LED_BUILTIN, OUTPUT);
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

		digitalWrite(PD6, val & 0b1);
		digitalWrite(PD5, val & 0b10);
		digitalWrite(PD4, val & 0b100);

		digitalToggle(LED_BUILTIN);
		//++val;
		delay_millis(1000);
	}
}

