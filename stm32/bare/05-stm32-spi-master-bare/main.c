#include <gpio.h>
#include <spi.h>
#include <usart.h>


u8 spi_transfer(u8 data)
{
	SPI1->DR; // read any previous data
	SPI1->DR = data; // write the data

	// wait until transmission complete
	while (!(SPI1->SR & SPI_SR_TXE));
	while (SPI1->SR & SPI_SR_BSY);

	return (u8)SPI1->DR; // return the result
}


void init_spi()
{
	RCC_APB2ENR |= RCC_APB2ENR_SPI1EN | RCC_APB2ENR_AFIOEN; 

	gpio_mode(PA5, OUTPUT_ALT);
	gpio_mode(PA6, INPUT);
	gpio_mode(PA7, OUTPUT_ALT);

	SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSTR ;

	// set the baud. If you don't get it right, you'll get rubbish results
	SPI1->CR1 &= ~(0b111<<3); // clear the baud rate
	u32 baud = 0b010 << 3;
	SPI1->CR1 |= baud;

	SPI1->CR1 |= SPI_CR1_SPE; // enable
	//SPI1->CR1 = 852;
	//SPI1->CR1 = 0b001101010100;
	//GPIOA->CRL = 0b10110100101100110100010001000100;
}


#define SS PA4

void main()
{
	gpio_mode(SS, OUTPUT);
	gpio_write(SS, HIGH);
	init_serial();
	init_spi();
	puts("simple spi example");
	beep();

	while(1) {
		u8 data = 0b01010101; // junk data to illustrate usage

		gpio_write(SS, LOW); //pull SS slow to prep other end for transfer
		data = spi_transfer(data);
		gpio_write(SS, HIGH); //pull ss high to signify end of data transfer

		print("stm32 master: slave returned: ");
		char buff[4];
		puts(itoa(data, buff, 10));

		for(int i=0; i< 600000; i++) nop(); // simple delay
	}
}




