/* 2021-06-06 mcarter started

Connections:
PB12 CS (we manage it ourselves)
PB13 SCLK
PB15 MOSI
MISO not required
*/
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/spi.h>

typedef uint8_t u8;
typedef uint32_t u32;

// shut the compiler up about missing protypes
void delay(int n);
void transfer_7219(uint8_t address, uint8_t value);

void delay(int n)
{
	for (int j = 0; j < n; j++)
	{
		for (int i = 0; i < 1000; i++)
		{
			__asm__("nop");
		}
	}
}


void transfer_7219(uint8_t address, uint8_t value) 
{
	gpio_clear(GPIOB, GPIO12);
	spi_xfer(SPI2, address); // seems to be roughly equiv of spi_send(SPI2, address); spi_read(SPI2);
	spi_xfer(SPI2, value); // seems to be roughly equiv of spi_send(SPI2, value); spi_read(SPI2);
	gpio_set(GPIOB, GPIO12);
}

int main(void)
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


	transfer_7219(0x0F, 0x00);
	transfer_7219(0x09, 0xFF); // Enable mode B
	transfer_7219(0x0A, 0x0F); // set intensity (page 9)
	transfer_7219(0x0B, 0x07); // use all pins
	transfer_7219(0x0C, 0x01); // Turn on chip

	u32 count = 0;
	while (1)
	{
		u32 num = count;
		for (uint8_t i = 0; i < 8; ++i)
		{
			u8 c = num % 10;
			num /= 10;
			u8 sep = 0; // thousands separator

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

			transfer_7219(i + 1, c);
			delay(1);
		}
		count++;
		delay(4000);
	}
}
