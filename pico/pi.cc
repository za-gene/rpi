#include "hardware/gpio.h"
#include "hardware/spi.h"
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

auto max7219_cs = 14;

void pi_max7219_init(void)
{
	pi_spi_init_std();
	pi_gpio_init(max7219_cs, OUTPUT);
	gpio_put(max7219_cs, 1);

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

		pi_max7219_tfr(i + 1, c);
		//delay(1);
	}

}

void pi_max7219_tfr(uint8_t address, uint8_t value)
{	
	gpio_put(max7219_cs, 0);
	uint8_t data[] = {address, value};
	spi_write_blocking(spi1, data, 2);
	gpio_put(max7219_cs, 1);
}

void pi_spi_init_std(void)
{
#define spi             spi1
#define PIN_SCK         10
#define PIN_MOSI        11
#define PIN_MISO        12
//#define PIN_CS          15


	auto spi_speed = 100'000; // slow to get initialisation
	spi_init(spi, spi_speed);
	gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
	gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
	gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
}
