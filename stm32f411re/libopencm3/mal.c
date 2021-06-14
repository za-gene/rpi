#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/spi.h>

#include "mal.h"


void mal_max7219_init(void)
{
	mal_spi_init_std();

	mal_max7219_tfr(0x0F, 0x00);
	mal_max7219_tfr(0x09, 0xFF); // Enable mode B
	mal_max7219_tfr(0x0A, 0x0F); // set intensity (page 9)
	mal_max7219_tfr(0x0B, 0x07); // use all pins
	mal_max7219_tfr(0x0C, 0x01); // Turn on chip
}

void mal_max7219_show_count(int count)
{
	uint32_t num = count;
	for (uint8_t i = 0; i < 8; ++i)
	{
		uint8_t c = num % 10;
		num /= 10;
		uint8_t sep = 0; // thousands separator

		// add in thousands separators
		if ((i > 0) && (i % 3 == 0))
		{
			sep = 1 << 7;
		}

		// blank if end of number
		if ((c == 0) && (num == 0) && (i > 0))
		{
			sep = 0;
			c = 0b1111;
		}

		c |= sep;

		mal_max7219_tfr(i + 1, c);
		//delay(1);
	}

}

void mal_max7219_tfr(uint8_t address, uint8_t value) 
{
	gpio_clear(GPIOB, GPIO12);
	spi_xfer(SPI2, address); // seems to be roughly equiv of spi_send(SPI2, address); spi_read(SPI2);
	spi_xfer(SPI2, value); // seems to be roughly equiv of spi_send(SPI2, value); spi_read(SPI2);
	gpio_set(GPIOB, GPIO12);
}

void mal_spi_init_std(void)
{
	rcc_periph_clock_enable(RCC_SPI2);
	//rcc_periph_clock_enable(RCC_USART1);
	//rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
	//rcc_periph_clock_enable(RCC_GPIOC);
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO13 | GPIO14 | GPIO15);
	gpio_set_af(GPIOB, GPIO_AF5, GPIO13 | GPIO14 | GPIO15);
	spi_init_master(SPI2, SPI_CR1_BAUDRATE_FPCLK_DIV_256, SPI_CR1_CPOL, SPI_CR1_CPHA, SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);
	//spi_enable_ss_output(SPI2); /* Required, see NSS, 25.3.1 section. */
	gpio_mode_setup(GPIOB,  GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12); // chip select
	spi_enable(SPI2);
}
