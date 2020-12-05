#include <mini_uart.h>
#include <stdio.h>
#include <timers.h>

#include "bcm2835.h"

void kernel_main(void)
{
	uart_init_as_stdio(115200);
	puts("bcm test");
	printf("hex test: %X\n", 62);
	bcm2835_init();
	puts("bcm initialised");
	bcm2835_spi_begin();
	puts("bcm spi started");

	// the following are defaults
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536);
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);

	puts("everything set up");

	while (1) {
		// Send a byte to the slave and simultaneously read a byte back from the slave
		// If you tie MISO to MOSI, you should read back what was sent
		uint8_t data = bcm2835_spi_transfer(0x23); // 0x23 is just a random number
		printf("Read from SPI: %d\n", data);
		delay_s(1);

	}
}
