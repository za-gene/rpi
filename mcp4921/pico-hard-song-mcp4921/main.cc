#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include <math.h>

#include "data.h"

using u8 = uint8_t;
using u16 = uint16_t;

#define	PIN_SCK		2
#define	PIN_MOSI	3
#define PIN_MISO 	4
#define	PIN_CS 		5

#define PULSE 18

//#define SAMPLES 100
//u16 sines[SAMPLES];

#define USE_POLL // using looping instead of timer callback to write to DAC

bool do_write = false;

void write_to_dac()
{
	if(!do_write) return;
	do_write = false;
	static int idx = 0;
	u16 src = data_bin[idx];
	src |= 0b0011'0000'0000'0000;
	spi_write16_blocking(spi0, &src, 1);
	idx++;
	if(idx == data_bin_len) idx = 0;
}

bool repeating_timer_callback(struct repeating_timer *t) 
{
	do_write = true;
#ifndef USE_POLL
	write_to_dac();
#endif

#if 0
	static bool pulse_on = false;
	pulse_on = ! pulse_on;
	gpio_put(PULSE, pulse_on);
#endif
	return true;
}

int main() 
{
	stdio_init_all();

	spi_init(spi0, 1'200'000);
	//spi_set_slave(spi0, true);
	spi_set_format(spi0, 16, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
	gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
	gpio_set_function(PIN_CS,   GPIO_FUNC_SPI);
	gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
	gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

	struct repeating_timer timer;
	const int freq = 44'100;
	add_repeating_timer_us(-1.0e6/freq, repeating_timer_callback, NULL, &timer);
	//add_repeating_timer_us(-23, repeating_timer_callback, NULL, &timer);

	//gpio_init(PULSE);
	//gpio_set_dir(PULSE, GPIO_OUT);

	for(;;) {
#ifdef USE_POLL
		write_to_dac();
#endif		
	}

	return 0;
}

