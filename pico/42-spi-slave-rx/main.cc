#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/adc.h"
//#include "hardware/clocks.h"
//#include "hardware/flash.h"
#include "hardware/gpio.h"
//#include "hardware/irq.h"
//#include "hardware/pwm.h"
//#include "hardware/spi.h"
// #include "tusb.h" // if you want to use tud_cdc_connected()

#include "pi.h"


/*
typedef struct spi_t {
	spi_inst_t* inst;
	uint sck;
	uint cs;
} spi_t;

enum spi_type_e {
	SPI_MASTER_TX // master mode, transmit only
}; 

void pi_spi_init(spi_t* spi, uint sck, uint cs, spi_type_e SPI_MASTER_TX)
{
	spi->inst = inst;
	spi->scl = sck;
	gpio_set_function(sck,  GPIO_FUNC_SPI); // sck
	spi->cs = cs;

	switch(SPI_MASTER_TX) {
		case SPI_MASTER_TX: 
			gpio_set_function(sck+1, GPIO_FUNC_SPI); // mosi
			//gpio_set_function(sck+2, GPIO_FUNC_SPI); // miso, not requires
			gpio_put(cs, 1);
			pi_gpio_init(cs, OUTPUT);
			break;
	}


}
*/

int main() 
{
	stdio_init_all();
	// while(!tud_cdc_connected()) sleep_ms(250); // wait for usb serial 

	//spi_t max7219;
	//pi_spi_init(&max7219, spi0, 2, 5);
	
	pi_max7219_init();

	uint32_t count = 0;
	while(1) {
		pi_max7219_show_count(count++);
		sleep_ms(1000);
	}


#define BTN  14 // GPIO number, not physical pin
#define LED  25 // GPIO of built-in LED
	gpio_init(BTN);
	gpio_set_dir(BTN, GPIO_IN);
	gpio_pull_up(BTN);
	// gpio_get() gets state of pin

	gpio_init(LED);
	gpio_set_dir(LED, GPIO_OUT);

	int i = 0;
	for(;;) {
		printf("Hello number %d\n", i++);
		gpio_put(LED, 1);
		sleep_ms(100);
		gpio_put(LED, 0);
		sleep_ms(1000);		
	}

	return 0;
}

