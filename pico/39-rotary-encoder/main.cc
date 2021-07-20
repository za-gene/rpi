#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"

#include "pi.h"
#include "rotary.h"

#define SIA 	21
#define SIB 	20
#define SW	19 

#define LED  25 // GPIO of built-in LED

int main() 
{
	stdio_init_all();
	// while(!tud_cdc_connected()) sleep_ms(250); // wait for usb serial 

	Rotary rot(SIA, SIB, SW);
	pi_gpio_init(LED, OUTPUT);

	Pulse pulse(500);
	int count = 0;
	int count_num = 0;
	while(1) {
		if(rot.sw_falling()) {
			puts("Rotary button pushed");
		}
		if(pulse.expired())
			pi_gpio_toggle(LED);
		if(int chg = rot.change()) {
			count += chg;
			printf("count %d = %d\n", ++count_num, count);
		}
	}
}

