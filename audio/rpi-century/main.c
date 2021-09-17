#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <bcm2835.h>

void intHandler(int dummy)
{
	puts("Exiting");
	bcm2835_spi_end();
	bcm2835_close();
	exit(0);
}


int main()
{
	puts("Type Ctrl-C to exit");
	signal(SIGINT, intHandler);

	if (!bcm2835_init()) {
		perror("Couldn't init BCM. Exiting");
		return 1;
	}

	bcm2835_spi_begin();

	// the following are defaults
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536);
	//bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256);
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	//bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
	int count = 1;



	while(1) {
		//usleep(3000);
		//printf("Sending %d\n", count);
		uint8_t rx = bcm2835_spi_transfer(count);
		printf("Received %d\n", rx);
		//if((rx&1)==0) continue;
		count++;
		if(count == 100) count = 1;
		//printf("Read number %d from SPI: %d\n", count++, data);
		sleep(1);

	}

	return 0;
}

