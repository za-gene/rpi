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

#include "../../1306/pico-sdk/oled.h"

#define	PIN_SCK		10
#define	PIN_MOSI	11
#define PIN_MISO 	12
#define	PIN_CS 		15

void init_card()
{
	// standard spi stuff
	int spi_speed = 1'200'000;
	spi_speed = 600'000;
	spi_init(spi1, spi_speed);
	//spi_set_slave(spi0, true);
	//spi_set_format(spi0, 16, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
	gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
	//gpio_set_function(PIN_CS,   GPIO_FUNC_SPI);
	gpio_set_dir(PIN_CS, GPIO_OUT);
	gpio_put(PIN_CS, 1);
	gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
	gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

}

int main() 
{
	stdio_init_all();
	// while(!tud_cdc_connected()) sleep_ms(250); // wait for usb serial 

	init_display(64, 6);
	//init_spi();
	init_card();
	ssd1306_print("1");
	show_scr();


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

