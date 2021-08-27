#include "hardware/dma.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"

#include "mcp4921-dma.h"



static int chan;

void mcp4921_dma_init(void)
{
#define	PIN_SCK		2
#define	PIN_MOSI	3
#define PIN_MISO 	4
#define	PIN_CS 		5
	int spi_speed = 17000000;
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
	//channel_config_set_read_increment(&cfg, false);
	//channel_config_set_write_increment(&cfg, false);
	dma_channel_configure(
			chan,          // Channel to be configured
			&cfg,            // The configuration we just created
			&spi_get_hw(spi0)->dr,           // The initial write address
			0,           // The initial read address
			1, // Number of transfers
			false           // Start immediately?
	);
	dma_channel_start(chan);

}

void mcp4921_dma_put(uint16_t vol)
{
	if(vol>4095) vol = 4095;
	vol |= (0b11 <<12);
	//gpio_put(HARE, 1);
	volatile uint16_t vol1 = vol; // volatile seems necessary
	dma_channel_transfer_from_buffer_now(chan, &vol1, 1); // assume tfr happens fast enough
	//dma_channel_wait_for_finish_blocking(chan);
	//gpio_put(HARE, 0);
}
