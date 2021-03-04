#include <fstream>
#include <sstream>
#include <unistd.h>

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
	string song{slurp("song16k.raw")};

	bcm2835_init();
	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64);    // ~ 4 MHz
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_16);    
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_1024);    // 390kHz on Pi3. Works
	//bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_512);
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default

	// set rising edge detection for gpio 21, pyhsical 40
	int trigger = 21;
	bcm2835_gpio_fsel(trigger, BCM2835_GPIO_FSEL_INPT);
	//bcm2835_gpio_ren(trigger);

	//int i = 0;
	for(unsigned char c : song) {
		//while(!bcm2835_gpio_eds(trigger)) usleep(10);
		// time to put out a new value
		//printf(".");
		//fflush(stdout);
		//bcm2835_gpio_set_eds(trigger); // clearing event detection so we can be triggered again

		while(bcm2835_gpio_lev(trigger) == HIGH);

#if 0
		dac_write((uint16_t) c << 4);
#else
		bcm2835_spi_writenb((char*) &c, 1);
#endif
		while(bcm2835_gpio_lev(trigger) == LOW);
		//dac_write(i);
		//i = 1 - i;
		//bcm2835_delayMicroseconds(125);
	}

	bcm2835_gpio_clr_ren(trigger);
	bcm2835_spi_end();
	bcm2835_close();
	return 0;
}

