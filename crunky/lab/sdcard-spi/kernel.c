#include <gpio.h>
#include <stdio.h>
#include <timers.h>

#define sleep_ms delay_ms

int spi_write_blocking (const uint8_t *src, size_t len)
{
	bcm2835_spi_write(src, len);
}

void gpio_put (uint gpio, bool value)
{
	if(value)
		bcm2835_gpio_set(gpio);
	else
		bcm2835_gpio_clr(gpio);
}


#define PIN_CS

int cs_low(void) 
{	
	gpio_put(PIN_CS, 0); 
	return 1;
}

void cs_high(void) { gpio_put(PIN_CS, 1); }

void kernel_main(void)
{
	sleep_ms(1);

	// spi pin setup
	long r;
	// GPIO_CD
	r=GPFSEL4; r&=~(7<<(7*3)); GPFSEL4=r;
	GPPUD=2; wait_cycles(150); GPPUDCLK1=(1<<15); wait_cycles(150); GPPUD=0; GPPUDCLK1=0;
	r=GPHEN1; r|=1<<15; GPHEN1=r;
	// GPIO_CLK, GPIO_CMD
	r=GPFSEL4; r|=(7<<(8*3))|(7<<(9*3)); GPFSEL4=r;
	GPPUD=2; wait_cycles(150); GPPUDCLK1=(1<<16)|(1<<17); wait_cycles(150); GPPUD=0; GPPUDCLK1=0;
	// GPIO_DAT0, GPIO_DAT1, GPIO_DAT2, GPIO_DAT3
	r=GPFSEL5; r|=(7<<(0*3)) | (7<<(1*3)) | (7<<(2*3)) | (7<<(3*3)); GPFSEL5=r;
	GPPUD=2; wait_cycles(150);
	GPPUDCLK1=(1<<18) | (1<<19) | (1<<20) | (1<<21);
	wait_cycles(150); GPPUD=0; GPPUDCLK1=0;

	// set card into spi mode: clock card at least 80 cycles with cs high
	printf("Set card to SPI mode ... ");
	cs_high();
	for(int i = 0; i < 10; i++) {
		uint8_t b = 0xFF;
		spi_write_blocking1(&b, 1);		
	}
	if(gpio_get(PIN_CS))
		printf("CS high. Bad juju.\n");
	else
		printf("CS low. Good.\n");

	while(1);

	const int bcm_pin = 26;
	gpio_sel(bcm_pin, OUTPUT); // set its pin mode to OUTPUT
	u32 delay = 250;
	while(1) {
		gpio_set(bcm_pin); // set the pin high
		delay_ms(delay);  
		gpio_clr(bcm_pin); // turn the pin off
		delay_ms(delay);
	}
}
