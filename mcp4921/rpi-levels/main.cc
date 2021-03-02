#include <fstream>
#include <sstream>
#include <bcm2835.h>


using namespace std;

std::string slurp(const char *filename)
{
	std::ifstream in;
	in.open(filename, std::ifstream::in | std::ifstream::binary);
	std::stringstream sstr;
	sstr << in.rdbuf();
	in.close();
	return sstr.str();
}

void dac_write(uint16_t value) {
	if(value>4095) value=4095;
	value |= 0b0011000000000000;
	char buf[2];
	buf[0] = value >>8;
	buf[1] = value & 0xff;
	bcm2835_spi_writenb(buf, 2);
}

int main()
{
	string song{slurp("song.raw")};

	bcm2835_init();
	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64);    // ~ 4 MHz
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_16);    
	//bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_1024);    
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default

	//int i = 0;
	for(unsigned char c : song) {
		dac_write((uint16_t) c << 4);
		//dac_write(i);
		//i = 1 - i;
		bcm2835_delayMicroseconds(125);
	}

	bcm2835_spi_end();
	bcm2835_close();
	return 0;
}

