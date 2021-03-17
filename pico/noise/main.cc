#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "hardware/xosc.h"
#include "hardware/structs/rosc.h"


#define OUT 16 // Speaker where we output noise

int main() 
{
	//stdio_init_all();

	gpio_init(OUT);
	gpio_set_dir(OUT, GPIO_OUT);

	xosc_init(); // I think you need to enable the xosc before using random bit generator

	//io_rw_32 bit = 0;
	//int bit = 0;
	for(;;) {
		const int pause = 1'000'000/440/2;
		io_rw_32 bit = rosc_hw->randombit;
		//bit = 1 - bit;
		gpio_put(OUT, bit);
		//sleep_us(pause);
		//sleep_ms(500);
		sleep_us(10);
	}

	return 0;
}

