#include <gpio.h>
#include <spi.h>

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
}


#define rs_pin PB0
#define cs_pin PA4

#define digitalWrite gpio_write

void sendByte(int rs_val, int val) {
	digitalWrite(rs_pin, rs_val);
	digitalWrite(cs_pin, LOW);
	spi_transfer(val);
	digitalWrite(cs_pin, HIGH);  
	for(int i=0; i< 500; i++) nop(); // simple delay to allow for catchup
}


void main()
{
	gpio_mode(cs_pin, OUTPUT);
	gpio_write(cs_pin, HIGH);
	gpio_mode(rs_pin, OUTPUT);

	init_spi();

	u8 contrast = 0x70  | 0b1000; // from 0x7C
	u8 display = 0b1111; // ori 0x0F
	uint8_t cmds[] = {0x39, 0x1D, 0x50, 0x6C, contrast , 0x38, display, 0x01, 0x06};
	for(int i=0; i<sizeof(cmds); ++i) sendByte(LOW, cmds[i]);

	// now send some interesting output
	uint8_t msg[] = {'S', 'T', 'M', '3', '2'};
	for(int i=0; i<sizeof(msg); ++i) sendByte(HIGH, msg[i]);

	while(1);
}




