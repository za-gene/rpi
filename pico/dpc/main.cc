#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"


#define	PIN_SCK		2
#define	PIN_MOSI	3
#define PIN_MISO 	4
#define	PIN_CS 		5

#define BTN 14 // GPIO number, not physical pin
#define LED 25 // GPIO of built-in LED

int main() 
{
	stdio_init_all();
	puts("dpc started");
	//while(1) putchar('.');

	spi_init(spi0, 4'000'000);
	spi_set_slave(spi0, true);
	spi_set_format(spi0, 16, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST); 
	gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
	gpio_set_function(PIN_CS,   GPIO_FUNC_SPI);
	gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
	gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

	for(;;) {
		uint16_t rx, tx = 42;
		spi_read16_blocking(spi0, tx, &rx, 1);
		printf("Received %d\n", rx);
	}


	gpio_init(BTN);
	gpio_set_dir(BTN, GPIO_IN);
	gpio_pull_up(BTN);
	// gpio_get() gets state of pin

	gpio_init(LED);
	gpio_set_dir(LED, GPIO_OUT);

	for(;;) {
		gpio_put(LED, 1);
		sleep_ms(100);
		gpio_put(LED, 0);
		sleep_ms(1000);
	}

	return 0;
}

