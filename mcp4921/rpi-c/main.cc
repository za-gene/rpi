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
	//value &= 0b0000111111111111;
	if(value>4095) value=4095;
	value |= 0b0011000000000000;
	bcm2835_spi_transfernb((char*) &value, (char*) &value, 2); // we just overwrite value
	/*
	   SPI.beginTransaction(dac_spi_settings);
	   digitalWrite(DAC_CS, LOW);
	   SPI.transfer16(value);
	   digitalWrite(DAC_CS, HIGH);
	   SPI.endTransaction();
	   */
}

int main()
{
	string song{slurp("song.raw")};

	bcm2835_init();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64);    // ~ 4 MHz
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default

	for(char c : song) {
	}

	bcm2835_spi_end();
	bcm2835_close();
	return 0;
}

