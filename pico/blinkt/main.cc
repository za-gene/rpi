/*
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/spi.h"
#include "hardware/gpio.h"
*/

#include "pico/time.h"

#include "blinkt.h"

#define dataPin  17
#define clockPin 18


int main() 
{
	//stdio_init_all();
	blinkt_init(dataPin, clockPin);
	for(;;) {
		static int red = 0;
		red = 10 - red; // switch between on and off
		for (int i = 0; i < 8; ++i) {
			blinkt_show();
			sleep_ms(100);
			blinkt_set_pixel_colour(i, red, 0, 0);
		}
	}
	return 0;
}

