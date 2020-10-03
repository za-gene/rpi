//#include <blue.h>
//#include <stdio.h>
#include <stdbool.h>

#include <gpio.h>
#include <usart.h>

//#define HIGH 1
//#define LOW 0

u32 rs_pin = PB0;
u32 cs_pin = PA4;
void delay(uint16_t ms)
{
	static bool initialised = false;
	if(!initialised) {
		// timer setup
		RCC_APB1ENR |= RCC_APB1ENR_TIM4EN;
		TIM4->PSC=7999;
		TIM4->ARR=65535;
		TIM4->CR1 |= TIM_CR1_CEN;
		initialised = true;
	}

	/* According to https://bit.ly/2GXlmxd 
	 * you must define start and now as 32-bit integers rather 
	 * than 16- or 8-,
	 * otherwise the bridge creates duplications
	 */
	uint32_t start = TIM4->CNT;
	while(1) {
		uint32_t now = TIM4->CNT;
		if(now-start>=ms) break;
	}
}

#define SPI_SR_RXNE (1<<0)
#define SPI_SR_TXE (1<<1)
#define SPI_SR_BSY (1<<7)


/* rs_val = LOW to send command, HIGH to send ASCII char 
*/
void sendByte(int rs_val, u8 val) {
	gpio_write(rs_pin, rs_val);
	gpio_write(cs_pin, LOW);

	// transmit sequence on page 712, section 25.3.5
	SPI1->DR = (u32)val;
	while((SPI1->SR & SPI_SR_TXE) != 1); //wait until TXE=1
	while(!(SPI1->SR & SPI_SR_RXNE)); //wait until TXE=1
	while(SPI1->SR & SPI_SR_BSY); //wait until BSY=0
	(void)SPI1->DR;

	//delay(60);
	gpio_write(cs_pin, HIGH);  
	delay(60);
}

#define RCC_APB2ENR_AFIOEN (1<<0)
#define RCC_APB2ENR_SPI1EN (1<<12)

#define SPI_CR1_MSTR (1<<2)
#define SPI_CR1_SPE (1<<6)
#define SPI_CR1_SSI (1<<8)
#define SPI_CR1_SSM (1<<9)
#define SPI_CR1_BIDIMODE (1<<15)
#define SPI_CR1_BIDIOE (1<<14)

#define SPI_CR2_SSOE (1<<2)


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

	SPI1->CR1 = 852;
	//SPI1->CR1 = 0b001101010100;
	GPIOA->CRL = 0b10110100101100110100010001000100;
}

#define nop() asm volatile ("nop")

#define SS PA4

void main()
{
	gpio_mode(SS, OUTPUT);
	gpio_write(SS, HIGH);
	init_serial();
	init_spi();
	puts("simple spi example 1");
	putchar('\a');

	//SPI1->DR = 0;
	while(1) {
		u8 data = 0b01010101; // junk data to illustrate usage

		gpio_write(SS, LOW); //pull SS slow to prep other end for transfer
		data = spi_transfer(data);
		gpio_write(SS, HIGH); //pull ss high to signify end of data transfer

		//ser.print("stm32 master: slave returned: ");  ser.println(res);
		char buff[4];
		puts("stm32 master: slave returned: ");
		puts(itoa(data, buff, 10));

		// simple delay
		for(int i=0; i< 600000; i++) nop();
		//delay(1000);
	}
}




