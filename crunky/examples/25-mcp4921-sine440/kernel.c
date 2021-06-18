#include <bcm2835.h>
#include <uart0.h>
#include <gpio.h>
#include <math.h>
#include <stdio.h>
//#include <timers.h>



void mcp4921_init()
{
	bcm2835_init();
	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64);    // ~ 4 MHz
	//bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_16);
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_1024);
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default
}


void mcp4921_deinit()
{
	bcm2835_spi_end();
	bcm2835_close();
}

void mcp4921_write(uint16_t value)
{
	if(value>4095) value=4095;
	value |= 0b0011000000000000;
	char buf[2];
	buf[0] = value >>8;
	buf[1] = value & 0xff;
	bcm2835_spi_writenb(buf, 2);
}



#define SAMPLES 64
u16 sines[SAMPLES];


int kernel_main()
{
	//uart_init_as_stdio(115200);
	uart0_init_as_stdio();
	puts("mcp4921 sine 440Hz  example");
	mcp4921_init();
	puts("mcp4921 initialised");
	//printf("this is just a little test of printf (number %d)\n", 16);

	// initialise
	puts("setting some doubles ... ");
	const double hi = 4095, freq = 440;
	double lo = 1.0/3.3 * hi; // 1V
	lo = 0;
	puts("... done");
	for (int i = 0; i < SAMPLES; i++) {
		puts("Will it like trig?");
		double v = sin((double) i * 2.0 * 3.1412 / SAMPLES);
		puts("it liked doing sine");
		v = (v+1.0)*(hi-lo)/2.0 + lo;
		sines[i] = v;
		//printf("%d %d\n", i, sines[i]);
	}
	puts("sine table generated");

	while(1)  {
		for (int i = 0; i < SAMPLES; i++) {
			mcp4921_write(sines[i]);
			bcm2835_delayMicroseconds(1e6/freq/SAMPLES);
		}
	}

	return 0;
}

/*
void kernel_main(void)
{
	const int bcm_pin = 26;
	gpio_sel(bcm_pin, OUTPUT); // set its pin mode to OUTPUT
	u32 delay = 500;
	while(1) {
		gpio_set(bcm_pin); // set the pin high
		delay_ms(delay);  
		gpio_clr(bcm_pin); // turn the pin off
		delay_ms(delay);
	}
}
*/
