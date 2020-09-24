#include <stm8.h>

#define SPI_CR1 *(uchar*)0x005200
#define SPI_CR2 *(uchar*)0x005201
#define SPI_ICR *(uchar*)0x005202
#define SPI_SR *(uchar*)0x005203
#define SPI_DR *(uchar*)0x005204
#define SPI_CRCPR *(uchar*)0x005205 // CRC polynomial register
#define SPI_RXCRCR *(uchar*)0x005206 // Rx CRC register
#define SPI_TXCRCR 0*(uchar*)x005207 // Tx CRC register

#define SPI_CR1_MSTR (1<<2)
#define SPI_CR1_SPE (1<<6)

// SPI status register section 20.4.4 page 285
#define SPI_SR_RXNE (1<<0) 
#define SPI_SR_TXE (1<<1) 
#define SPI_SR_WKUP (1<<3)
#define SPI_SR_CRCERR (1<<4)
#define SPI_SR_MODF (1<<5) 
#define SPI_SR_OVR (1<<6) 
#define SPI_SR_BSY (1<<7) 

void main()
{
	// set speed to 2MHz/8 = 250,000
	SPI_CR1 |= (0b010 << 3); // 0b010 is bause rate f_MASTER/8, 3 is baud rate bits
	SPI_CR1 |= SPI_CR1_MSTR; // master configuration
	SPI_CR1 |= SPI_CR1_SPE; // enable peripheral

	//gpio_mode_out(BUILTIN_LED);
	//gpio_mode_pullup(BUTTON);

	int val = 0;
	SPI_DR = val; // 
	while (1)
	{
		while(!(SPI_SR & SPI_SR_RXNE));
		//u8 ch = SPI->DR;
		SPI_DR = ++val;
		//u8 high = gpio_read(BUTTON);
		//gpio_write(BUILTIN_LED, high);
	}
}

