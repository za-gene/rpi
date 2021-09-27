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

#define OUT 	26
#define S	8  // Set (active high)
#define R	12  // Reset (ative low)
int main() 
{
	pi_gpio_init(OUT, OUTPUT);
	pi_gpio_init(S, INPUT);
	//gpio_set_input_hysteresis_enabled(S, true); // i.e. schmitt trigger
	pi_gpio_init(R, INPUT_PULLUP);

	bool on = false;

	for(;;) {
		if(gpio_get(S)) on = true;
		if(!gpio_get(R)) on = false;
		gpio_put(OUT, on);
	}

	return 0;
}

