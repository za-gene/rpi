#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include <math.h>

#include "pi.h"

#define TOGGLE 16

double framerate = 44100, saw_freq = 500;
uint64_t period = 1'000'000/framerate;

int chan;

volatile uint16_t vol1;
void mcp4921_init(void)
{
#define	PIN_SCK		2
#define	PIN_MOSI	3
#define PIN_MISO 	4
#define	PIN_CS 		5
	int spi_speed = 18'000'000;
	spi_init(spi0, spi_speed);
	spi_set_format(spi0, 16, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
	//gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
	gpio_set_function(PIN_CS,   GPIO_FUNC_SPI);
	//gpio_init(PIN_CS);
	gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
	gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

	chan = dma_claim_unused_channel(true);
	dma_channel_config cfg = dma_channel_get_default_config(chan);
	channel_config_set_transfer_data_size(&cfg, DMA_SIZE_16);
	channel_config_set_read_increment(&cfg, false);
	channel_config_set_write_increment(&cfg, false);
	static uint32_t dummy = 0;
	dma_channel_configure(
			chan,          // Channel to be configured
			&cfg,            // The configuration we just created
			&spi_get_hw(spi0)->dr,           // The initial write address
			&vol1,           // The initial read address
			1, // Number of transfers; in this case each is 1 byte.
			false           // Start immediately?
	);
	dma_channel_start(chan);

}

void mcp4921_put(uint16_t vol)
{
	if(vol>4095) vol = 4095;
	vol |= (0b11 <<12);
	vol1 = vol;
#if 1
	dma_channel_transfer_from_buffer_now(chan, &vol1, 1); // assume tfr happens fast enough
	//dma_channel_wait_for_finish_blocking(chan);
#else
	spi_write16_blocking(spi0, (const uint16_t*) &vol1, 1);
#endif

}

volatile float y = 0, dy;

static void alarm_0_irq() 
{
	pi_alarm_rearm(0, period);
	mcp4921_put(y);
	y += dy;
	if(y>4095) y=0;
	pi_gpio_toggle(TOGGLE);
}

int main() 
{
	stdio_init_all();
	pi_gpio_init(TOGGLE, OUTPUT);
	dy = 4095 * saw_freq / framerate;
	mcp4921_init();
	pi_alarm_init(0, alarm_0_irq, period);

	while(1);
	return 0;
}

