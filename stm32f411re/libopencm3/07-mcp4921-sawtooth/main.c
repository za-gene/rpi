#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include "mal.h"


typedef uint32_t u32;

#define LED PC13

double framerate = 44100, saw_freq = 500;


void mcp4921_init(void);
void mcp4921_put(uint16_t vol);

/** @brief Initialise an MCP4921 DAC

Uses SPI2. It will not play well with other devices using SPI2
because it uses 16-bit transfer and high baudrate and uses
an interrupt to set the CS pin high again.

Pin connections: 
* PB12 CS
* PB13 SCK
* PB15 MOSI

PB14 MISO is set up, but not used.
*/

void mcp4921_init(void)
{
	rcc_periph_clock_enable(RCC_SPI2);
	rcc_periph_clock_enable(RCC_GPIOB);
	uint16_t mask = GPIO13 | GPIO14 | GPIO15;
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, mask);
	gpio_set_af(GPIOB, GPIO_AF5, mask);
	spi_init_master(SPI2, SPI_CR1_BAUDRATE_FPCLK_DIV_2,
		SPI_CR1_CPOL, SPI_CR1_CPHA, SPI_CR1_DFF_16BIT, SPI_CR1_MSBFIRST);
	gpio_mode_setup(GPIOB,  GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12); // chip select
	gpio_set(GPIOB, GPIO12);
	spi_enable(SPI2);
	spi_enable_rx_buffer_not_empty_interrupt(SPI2);
	nvic_enable_irq(NVIC_SPI2_IRQ);
}


void spi2_isr(void)
{
	SPI_DR(SPI2);
	gpio_set(GPIOB, GPIO12);
	gpio_toggle(GPIOC, GPIO13);
}


void mcp4921_put(uint16_t vol)
{
	if(vol>4095) vol = 4095;
	vol |= (0b11 <<12);
	gpio_clear(GPIOB, GPIO12);
	spi_write(SPI2, vol); // doesn't block. spi2_isr() sends the pin high again
}

volatile float y = 0, dy;

void tim2_isr(void)
{
	timer_clear_flag(TIM2, TIM_SR_UIF); // hmmm, seems to matter that it's at the top
	mcp4921_put(y);
	y += dy;
	if(y>4095) y=0;
}


int main(void)
{
	dy = 4095 * saw_freq / framerate;	
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_mode_setup(GPIOC,  GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);
	mcp4921_init();
	mal_timer_init(TIMER2, framerate);
	while(1);
}
