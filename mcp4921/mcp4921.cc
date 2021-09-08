//#include <fstream>
//#include <sstream>
#include <bcm2835.h>

#include "mcp4921.h"


//using namespace std;


mcp4921::mcp4921()
{
	bcm2835_init();
	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64);    // ~ 4 MHz
	//bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_16);    
	//bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_1024);    
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default
}


mcp4921::~mcp4921()
{
	bcm2835_spi_end();
	bcm2835_close();
}

void mcp4921::write(uint16_t value)
{
	if(value>4095) value=4095;
	const uint16_t cmd = 0b0111 << 12; // I usually use 0b0011 << 12
	//value |= 0b0011000000000000;
	value |= cmd;
	char buf[2];
	buf[0] = value >>8;
	buf[1] = value & 0xff;
	bcm2835_spi_writenb(buf, 2);
}

