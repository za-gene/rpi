#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include "mal.h"


typedef uint32_t u32;

#define LED PC13

double framerate = 44100, saw_freq = 500;

pin_t* cs_mcp4921 = PB12;

void mal_mcp4921_init(void);
void mal_mcp4921_set(uint16_t vol);

/** @brief Initialise an MCP4921 DAC

Uses SPI2. It might not play well with other devices using SPI2
because it uses 16-bit transfer and high baudrate.

Pin connections: 
* PB12 CS
* PB13 SCK
* PB15 MOSI

PB14 MISO is set up, but not used.
*/

void mal_mcp4921_init(void)
{
	//pin_out(cs_mcp4921);
	//pin_high(cs_mcp4921);
	//mal_spi_init_std();
	rcc_periph_clock_enable(RCC_SPI2);
	rcc_periph_clock_enable(RCC_GPIOB);
	//uint12_t 
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO13 | GPIO14 | GPIO15);
	gpio_set_af(GPIOB, GPIO_AF5, GPIO13 | GPIO14 | GPIO15);
	spi_init_master(SPI2, SPI_CR1_BAUDRATE_FPCLK_DIV_2,
		SPI_CR1_CPOL, SPI_CR1_CPHA, SPI_CR1_DFF_16BIT, SPI_CR1_MSBFIRST);
	//spi_enable_ss_output(SPI2); /* Required, see NSS, 25.3.1 section. */
	//gpio_mode_setup(GPIOB,  GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12); // chip select
	//gpio_set(GPIOB, GPIO12);
	pin_out(cs_mcp4921);
	pin_high(cs_mcp4921);
	spi_enable(SPI2);
}


void mal_mcp4921_set(uint16_t vol)
{
	if(vol>4095) vol = 4095;
	vol |= (0b11 <<12);
	//uint16_t val = (vol >>8);
	pin_low(cs_mcp4921);
	//spi_send(SPI2, vol);	
	//spi_xfer(SPI2, val);
	//val = vol & 0xFF;
	//spi_xfer(SPI2, val);
	spi_xfer(SPI2, vol); // will wait for data to be fully transmitted
	pin_high(cs_mcp4921);
}

volatile float y = 0, dy;

void tim2_isr(void) // the standard ISR name for TIM2
{
	timer_clear_flag(TIM2, TIM_SR_UIF); // hmmm, seems to matter that it's at the top
	mal_mcp4921_set(y);
	y += dy;
	if(y>4095) y=0;
	pin_toggle(LED);
}


int main(void)
{
	dy = 4095 * saw_freq / framerate;	
	pin_out(PC13);
	mal_mcp4921_init();
	mal_timer_init(TIMER2, framerate);
	while(1);
}
