#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include <math.h>

#include "pi.h"


using u8 = uint8_t;
using u16 = uint16_t;


#define LED 25


double framerate = 44100, saw_freq = 500;
uint64_t period = 1'000'000/framerate;


void mcp4921_init(void)
{
#define	PIN_SCK		2
#define	PIN_MOSI	3
#define PIN_MISO 	4
#define	PIN_CS 		5
	int spi_speed = 18'000'000;
	spi_init(spi0, spi_speed);
	spi_set_format(spi0, 16, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
	gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
	gpio_set_function(PIN_CS,   GPIO_FUNC_SPI);
	gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
	gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
}

void mcp4921_put(uint16_t vol)
{
	if(vol>4095) vol = 4095;
	vol |= (0b11 <<12);
	spi_write16_blocking(spi0, &vol, 1);
}

volatile float y = 0, dy;

static void alarm_0_irq() 
{
	pi_alarm_rearm(0, period);
	mcp4921_put(y);
	y += dy;
	if(y>4095) y=0;
	pi_gpio_toggle(LED);
}

int main() 
{
	stdio_init_all();
	pi_gpio_init(LED, OUTPUT);
	dy = 4095 * saw_freq / framerate;
	mcp4921_init();
	pi_alarm_init(0, alarm_0_irq, period);

	while(1);
	return 0;
}

