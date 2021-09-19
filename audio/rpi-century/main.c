#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <bcm2835.h>

typedef uint8_t u8;

void intHandler(int dummy)
{
	puts("Exiting");
	bcm2835_spi_end();
	bcm2835_close();
	exit(0);
}


void cmd(u8 type, u8 channel)
{
	u8 b = 0b10000000;
	b = b | (type<<4) | channel;
	bcm2835_spi_writenb(&b, 1);
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
	const int khz = 1000 ;
	const int mhz = khz * khz;
	int speed = 500 * khz;
	bcm2835_spi_set_speed_hz(speed);
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	//bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
	uint8_t tx = 1, rx;


	// send note 440 Hz
	uint8_t b;
	for(int i = 0; i< 8; i++) {
		cmd(1, 0); // note on
		//b = 0b10010000;
		//bcm2835_spi_writenb(&b, 1 ); // note on
		b = 69;
		bcm2835_spi_writenb(&b, 1 ); // note on
		bcm2835_delay(500); 
		cmd(0, 0); //off
		bcm2835_delay(500); 
	}
	return 0;

	return 0;
}

