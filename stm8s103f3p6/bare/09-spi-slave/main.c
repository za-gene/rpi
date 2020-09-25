#include <stm8.h>
//#include <millis.h>

// SPI Interrupt Control Register section 20.4.3 page 284
#define SPI_ICR_TXIE (1<<7)
#define SPI_ICR_RXIE (1<<6)

#define SPI_CR2_SSM (1<<1)

#define CS PA3 // SPI chip select
#define MISO PC7

#if 1
volatile u8 val =0;
void my_spi_handler() __interrupt(SPI_ISR)
{
		//gpio_toggle(BUILTIN_LED);
	if(SPI_SR & SPI_SR_RXNE) { // seems necessary
		u8 byte = SPI_DR;
		//SPI_DR = val++;
		//gpio_toggle(BUILTIN_LED);
	}

	if(SPI_SR & SPI_SR_TXE) { // master is ready for transmission
		SPI_DR = val++;
		gpio_toggle(BUILTIN_LED);
	}
}
#endif

void main()
{
	enable_interrupts();
	//gpio_mode_out(PD4);
	//gpio_mode_out(PD5);
	//gpio_mode_out(PD6);
	//gpio_mode_in(CS);
	//gpio_mode_out(MISO);
	gpio_mode_out(BUILTIN_LED);
	gpio_write(BUILTIN_LED, 1);
	//gpio_write(CS, 1);
	//init_millis();

	SPI_CR1 &= ~SPI_CR1_SPE; // disable SPI
	// set speed to 2MHz/8 = 250,000
	//SPI_CR1 |= (0b010 << 3); // 0b010 is bause rate f_MASTER/8, 3 is baud rate bits
	//SPI_CR1 |= SPI_CR1_MSTR; // master configuration
	SPI_ICR |= SPI_ICR_TXIE;
	SPI_ICR |= SPI_ICR_RXIE; // seems necessary
	SPI_CR2 |= SPI_CR2_SSM; // Slave mangaement performed by software
	SPI_CR1 |= SPI_CR1_SPE; // enable peripheral

	u8 val = 0;
	//SPI_DR = val;  
	while (1)
	{
		continue;
		//gpio_write(CS, 0);
		//while(!(SPI_SR & SPI_SR_RXNE));
		//while(!(SPI_SR & SPI_SR_TXE));
		u8 dummy = SPI_DR;
		SPI_DR = ++val; // seems necessary
		//gpio_write(CS, 1);

		//gpio_write(PD6, val & 0b1);
		//gpio_write(PD5, val & 0b10);
		//gpio_write(PD4, val & 0b100);
		
		gpio_toggle(BUILTIN_LED);
		//++val;
		//delay_millis(1000);
	}
}

