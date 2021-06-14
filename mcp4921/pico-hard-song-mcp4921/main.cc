#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include <math.h>

#include "pi.h"


#include "data.h"

using u8 = uint8_t;
using u16 = uint16_t;

#define	PIN_SCK		2
#define	PIN_MOSI	3
#define PIN_MISO 	4
#define	PIN_CS 		5

#define LED 19

uint64_t period;


void write_to_dac()
{
	static int idx = 0;
	u8 datum = data_bin[idx];
	u16 src = datum;
	src <<= 4;
	if(src>4095) src = 4095;
	src |= 0b0011'0000'0000'0000;
	spi_write16_blocking(spi0, &src, 1);
	idx++;
	if(idx == data_bin_len) idx = 0;
}

static void alarm_0_irq() 
{
	pi_alarm_rearm(0, period);
	gpio_put(LED, !gpio_get(LED));
	write_to_dac();
}

int main() 
{
	stdio_init_all();
	gpio_init(LED);
	gpio_set_dir(LED, GPIO_OUT);

	int spi_speed = 1'200'000;
	//spi_speed = 600'000;
	spi_init(spi0, spi_speed);
	//spi_set_slave(spi0, true);
	spi_set_format(spi0, 16, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
	gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
	gpio_set_function(PIN_CS,   GPIO_FUNC_SPI);
	gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
	gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

	struct repeating_timer timer;
	double freq = 44'100;
	//freq = 8'000;
	//freq = 22'000;
	period = 1'000'000/freq;
	pi_alarm_init(0, alarm_0_irq, period);

	while(1);
	return 0;
}

