#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/spi.h"
#include "hardware/gpio.h"
//#include <type_traits>


#include "i2cmc.h"
#include "ledmat.h"


#define BTN 14 // GPIO number, not physical pin
#define LED 25 // GPIO of built-in LED



I2C i2c(i2c1, 26);
LedMat ledmat(i2c);

static uint8_t  pattern1[] = { 
	0b1000'0001,
	0b0100'0010,
	0b0010'0100,
	0b0001'0000,

	0b0000'1000,
	0b0010'0100,
	0b0100'0010,
	0b1000'0001
};

static uint8_t  pattern2[] = { // the letter P, with some orientation frills
	0b1111'0001,
	0b1000'1000,
	0b1000'1000,
	0b1111'0000,

	0b1000'0000,
	0b1000'0000,
	0b1000'0001,
	0b1000'0010
};

int main() 
{
	stdio_init_all();

	//initalise display
	for(int r=0; r<8; r++) {
		uint8_t row = pattern2[r];
		for(int c=0; c<8; c++) {
			ledmat.set(r,c, row >> 7);
			row <<=1;
		}
	}
	ledmat.show();


	gpio_init(BTN);
	gpio_set_dir(BTN, GPIO_IN);
	gpio_pull_up(BTN);

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

