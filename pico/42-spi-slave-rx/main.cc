#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/adc.h"
//#include "hardware/clocks.h"
//#include "hardware/flash.h"
#include "hardware/gpio.h"
//#include "hardware/irq.h"
//#include "hardware/pwm.h"
#include "hardware/spi.h"
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

#define SLAVE_SCK 	2
#define SLAVE_MOSI	3
#define SLAVE_CS	5

#define LED		25

volatile uint32_t count = 0;


void rising(uint gpio, uint32_t events)
{
	if(gpio == SLAVE_CS) {
		pi_gpio_toggle(LED);
		pi_max7219_show_count(count);
		printf("count = %d\n", count);
	}

	if(gpio == SLAVE_SCK) {
		count <<= 1;
	       	if(gpio_get(SLAVE_MOSI)) count |= 1;
	}
}

int main() 
{
	stdio_init_all();
	pi_gpio_init(LED, OUTPUT);
	pi_max7219_init();
	pi_max7219_show_count(0);
	printf("Running spi-slave-rx\n");

	pi_gpio_init(SLAVE_SCK, INPUT);
	gpio_set_irq_enabled_with_callback(SLAVE_SCK, GPIO_IRQ_EDGE_RISE, true, &rising);
	pi_gpio_init(SLAVE_MOSI, INPUT);
	pi_gpio_init(SLAVE_CS, INPUT);
	gpio_set_irq_enabled_with_callback(SLAVE_CS, GPIO_IRQ_EDGE_RISE, true, &rising); // per SDK: gpio is ignored

	while(1);
}

