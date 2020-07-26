#include "delays.h"
#include "gpio.h"


void kernel_main(void)
{
	const int bcm_pin = 24; // physical pin 18, GPIO pin 24
	gpio_sel(bcm_pin, OUTPUT); // set its pin mode to OUTPUT
	while(1) {
		gpio_set(bcm_pin); // set the pin high
		delay_ms(500);  // wait 0.5 secs
		gpio_clr(bcm_pin); // turn the pin off
		delay_ms(1000); // wait 0.5 secs
	}
}
