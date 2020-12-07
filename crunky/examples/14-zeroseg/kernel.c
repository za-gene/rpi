#include <bcm2835.h>
#include <mini_uart.h>
#include <stdio.h>
#include <timers.h>


void transfer_7219(uint8_t address, uint8_t value);
void init_7219();

void kernel_main(void)
{
	uart_init_as_stdio(115200);
	puts("\nzeroseg example");
	bcm2835_init();
	bcm2835_spi_begin();
	init_7219();

	/*
	// the following are defaults
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536);
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
	*/

	int idx =0;
	while (1) {
		int r = idx;
		for (uint8_t i = 0; i < 8; ++i) {
			transfer_7219(i+1, r % 10 );
			r /= 10;
			delay_ms(1);
		}
		delay_s(1);
		idx++;
	}
}
