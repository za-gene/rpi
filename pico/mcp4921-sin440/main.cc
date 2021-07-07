#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include <math.h>

#include "pi.h"


using u8 = uint8_t;
using u16 = uint16_t;

#define	PIN_SCK		2
#define	PIN_MOSI	3
#define PIN_MISO 	4
#define	PIN_CS 		5

#define PULSE 18

#define SAMPLES 100
u16 sines[SAMPLES];

#define USE_POLL // using looping instead of timer callback to write to DAC

bool do_write = false;

uint8_t cs_mcp4921 = 5;

void pin_low(uint8_t pin)
{
	gpio_put(pin, 0);
}

void pin_high(uint8_t pin)
{
	gpio_put(pin, 1);
}


void mcp4921_init()
{
	pi_spi_init_std();
	pi_gpio_init(cs_mcp4921, OUTPUT);
	pin_high(cs_mcp4921);
}

void mcp4921_write(uint16_t vol)
{
	spi_set_baudrate(spi1, 1'200'000);
	pin_low(cs_mcp4921);
	//static int idx = 0;
	//u16 src = sines[idx];
	if(vol>4095) vol = 4095;
	vol |= 0b0011'0000'0000'0000;
	uint16_t hi = vol >>8, lo = vol & 0xFF;
	uint8_t data[] = { (uint8_t)hi, (uint8_t)lo};
	spi_write_blocking(spi1, data, 2);
	//spi_write_blocking(spi, data, 2);

	//spi_write16_blocking(spi0, &src, 1);
	//idx++;
	//if(idx == SAMPLES) idx = 0;
	pin_high(cs_mcp4921);
}


void write_to_dac()
{
        static int idx = 0;
        u16 src = sines[idx];
	mcp4921_write(src);
	idx++;
	if(idx == SAMPLES) idx = 0;
}

bool repeating_timer_callback(struct repeating_timer *t) 
{
#ifndef USE_POLL
	write_to_dac();
#endif
	do_write = true;

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
	mcp4921_init();


	/*
	spi_init(spi0, 1'200'000);
	//spi_set_slave(spi0, true);
	spi_set_format(spi0, 16, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
	gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
	gpio_set_function(PIN_CS,   GPIO_FUNC_SPI);
	gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
	gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
	*/

	for (int i = 0; i < SAMPLES; i++) {
		double v = (sin((double) i * 2.0 * 3.1412 / SAMPLES) + 1.0) * 4096.0 / 2.0;
		if(v>=4096) v = 4095;
		if(v<0) v = 0 ;
		//v = clamp(0, 4095);
		sines[i] = v;
		//sines[i] |= 0b0011'0000'0000'0000; // for the DAC instruction
		//ser.println(sines[i]);
	}

	struct repeating_timer timer;
	const int freq = 440;
	add_repeating_timer_us(-1.0e6/SAMPLES/freq, repeating_timer_callback, NULL, &timer);
	//add_repeating_timer_us(-23, repeating_timer_callback, NULL, &timer);

	gpio_init(PULSE);
	gpio_set_dir(PULSE, GPIO_OUT);

	//int idx = 0;
	for(;;) {

#ifdef USE_POLL
		if(do_write) {
			do_write = false;
			write_to_dac();
		}
#endif		
		//sleep_ms(1000);
	}

	return 0;
}

