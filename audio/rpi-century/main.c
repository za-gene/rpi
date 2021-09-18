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
	//bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536);
	//bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64); // 6.25 MHz on Pi3
	//bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256);

	//bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_512);
	//bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_1024);
	//bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_2048);
	//bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_4096); // too fast
	//bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32768); //works
	//bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_16384);
	//bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_8192); // works
	const int mhz = 1000 * 1000;
	bcm2835_spi_set_speed_hz(1*mhz);
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	//bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
	uint8_t tx = 1, rx;


	// send note 440 Hz
	uint8_t b;
	for(int i = 0; i< 8; i++) {
		b = 0b10010000;
		bcm2835_spi_writenb(&b, 1 ); // note on
		//bcm2835_delay(2); 
		b = 73;
		bcm2835_spi_writenb(&b, 1 ); // note on
		bcm2835_delay(500); 
		b = 0b10000000;
		bcm2835_spi_writenb(&b, 1 ); // note on
		bcm2835_delay(500); 
	}
	return 0;



	while(1) {
		//printf("Sending %d\n", count);
		while(1) {
			//usleep(1);
			bcm2835_delayMicroseconds(10);
			rx = bcm2835_spi_transfer(tx);
			//break;
			if(rx != 0) break;
		}
		tx++;
		if(tx==100) tx = 1;
	}

	return 0;
}

