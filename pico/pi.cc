#include "hardware/gpio.h"
#include "hardware/timer.h"

#include "pi.h"

void pi_alarm_rearm(int alarm_num, uint64_t delay_us)
{
	// Clear the alarm irq
	hw_clear_bits(&timer_hw->intr, 1u << alarm_num);

	//uint32_t delay_us = 2 * 1'000'000; // 2 secs
	// Alarm is only 32 bits so if trying to delay more
	// than that need to be careful and keep track of the upper
	// bits
	uint64_t target = timer_hw->timerawl + delay_us;

	// Write the lower 32 bits of the target time to the alarm which
	// will arm it
	timer_hw->alarm[alarm_num] = (uint32_t) target;

}

void pi_alarm_init(uint alarm_num, irq_handler_t callback, uint64_t delay_us)
{
	hw_set_bits(&timer_hw->inte, 1u << alarm_num); // enable interrupt for alarm
	int irq_num = 0;
	switch(alarm_num) {
		case 0: irq_num = TIMER_IRQ_0 ; break;
		case 1: irq_num = TIMER_IRQ_1 ; break;
		case 2: irq_num = TIMER_IRQ_2 ; break;
		case 3: irq_num = TIMER_IRQ_3 ; break;
		default: assert(false);
	}
	irq_set_exclusive_handler(irq_num, callback);
        irq_set_enabled(irq_num, true);
	pi_alarm_rearm(alarm_num, delay_us);
}

void pi_gpio_init(uint gpio, pi_gpio_mode_e mode)
{
	gpio_init(gpio);
	switch(mode) {
		case INPUT_PULLUP:
			gpio_pull_up(gpio);
			[[fallthrough]];
		case INPUT:
			gpio_set_dir(gpio, GPIO_IN);
			break;
		case OUTPUT:
			gpio_set_dir(gpio, GPIO_OUT);
			break;
		default:
			assert(false);
	}
}

void pi_gpio_toggle(uint gpio)
{
	gpio_put(gpio, !gpio_get(gpio));
}



void pi_max7219_init(void)
{
	pi_spi_init_std();

	pi_max7219_tfr(0x0F, 0x00);
	pi_max7219_tfr(0x09, 0xFF); // Enable mode B
	pi_max7219_tfr(0x0A, 0x0F); // set intensity (page 9)
	pi_max7219_tfr(0x0B, 0x07); // use all pins
	pi_max7219_tfr(0x0C, 0x01); // Turn on chip
}

void pi_max7219_show_count(int count)
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

void pi_max7219_tfr(uint8_t address, uint8_t value)
{
	gpio_clear(GPIOB, GPIO12);
	spi_xfer(SPI2, address); // seems to be roughly equiv of spi_send(SPI2, address); spi_read(SPI2);
	spi_xfer(SPI2, value); // seems to be roughly equiv of spi_send(SPI2, value); spi_read(SPI2);
	gpio_set(GPIOB, GPIO12);
}

void pi_spi_init_std(void)
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
