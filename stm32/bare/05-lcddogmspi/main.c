//#include <blue.h>
//#include <stdio.h>
#include <stdbool.h>

#include <gpio.h>

#define HIGH 1
#define LOW 0

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
	while(SPI1->SR & SPI_SR_BSY); //wait until BSY=0
	//(void)SPI1->DR;

	//delay(60);
	gpio_write(cs_pin, HIGH);  
	delay(60);
}

#define RCC_APB2ENR_SPI1EN (1<<12)

#define SPI_CR1_SPE (1<<6)
#define SPI_CR1_SSM (1<<9)
#define SPI_CR1_MSTR (1<<2)
#define SPI_CR1_BIDIMODE (1<<15)
#define SPI_CR1_BIDIOE (1<<14)

#define SPI_CR2_SSOE (1<<2)
u32 pin_to_gpio_1(u32 pin)
{
	u32 port = pin>>4;
	return GPIO_BASE + port*0x400;
}

void gpio_mode_alt_out(u32 pin)
{
	u32 port = pin >>4; // gives 0 for Port A, 1 for port B, 2 for port C

	// enable the port only if it not enabled
	//u32 port_enabled = RCC_APB2ENR & (1 << (port+2));
	//if(port_enabled == 0) 
	RCC_APB2ENR |= (1 << (port+2)); // enable port

	u32 GPIOx_CRx = pin_to_gpio_1(pin) + 0x00; // assume GPIOx_CRL
	u32 pin_num = pin & 0x0F;
	if(pin_num > 7) {
		GPIOx_CRx += 0x04; // bump to GPIOx_CRH;
		pin_num -= 8;
	}
	u32 mask = 0b1111 << (pin_num*4);
	*(volatile u32*) GPIOx_CRx &= ~mask; // mask out the mode and CNF
	*(volatile u32*) GPIOx_CRx |= (0b1010<< (pin_num*4)) ; //  CNF output push-pull, max speed 2MHz
	//GPIOC->CRH   |= 0x00200000;
	//gpio_write(pin, 0);

}

void main() {
	gpio_mode_out(cs_pin);
	gpio_write(cs_pin, 1);
	gpio_mode_out(rs_pin);
	gpio_mode_alt_out(PA5);
	gpio_mode_alt_out(PA7);

	// setup SPI as master, transmit only
	RCC_APB2ENR |= (1<<2); // enable port A, where our SPI is
	RCC_APB2ENR |= RCC_APB2ENR_SPI1EN; // enable SPI1
	SPI1->CR1 |= SPI_CR1_BIDIMODE // 1-line bi-directional
		| SPI_CR1_BIDIOE // transmit only
		| SPI_CR1_SSM // we'll manage CS pin ourselves
		| SPI_CR1_MSTR // master mode
		| (0b010 <<3) // Baud rate. Just a guess for now
		;
	SPI1->CR2 |= SPI_CR2_SSOE; // some bizarre output enabling
	SPI1->CR1 |= SPI_CR1_SPE; // enable SPI

	u8 contrast = 0x70  | 0b1000; // from 0x7C
	u8 display = 0b1111; // ori 0x0F
	u8 cmds[] = {0x39, 0x1D, 0x50, 0x6C, contrast , 0x38, display, 0x01, 0x06};
	for(int i=0; i<sizeof(cmds); ++i) sendByte(LOW, cmds[i]);
	//Serial.println(sizeof(cmds));

	// now send some intersting output
	u8 msg[] = {'S', 'T', 'M', '3', '2'};
	for(int i=0; i<sizeof(msg); ++i) sendByte(HIGH, msg[i]);
	//Serial.println(sizeof(msg));

	while(1);
}



