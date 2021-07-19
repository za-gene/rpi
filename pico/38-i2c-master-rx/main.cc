#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/adc.h"
//#include "hardware/clocks.h"
//#include "hardware/flash.h"
#include "hardware/gpio.h"
//#include "hardware/irq.h"
//#include "hardware/pwm.h"
#include "hardware/i2c.h"
// #include "tusb.h" // if you want to use tud_cdc_connected()

#include "pi.h"

#define LED  25 // GPIO of built-in LED



int main() 
{
	stdio_init_all();
	// while(!tud_cdc_connected()) sleep_ms(250); // wait for usb serial 

	pi_i2c_init(4);
	pi_gpio_init(LED, OUTPUT);
	const auto addr = 0x4;
	uint8_t value = 0;
	for(;;) {
		int res = i2c_read_blocking(pi_i2c_default_port, addr, &value, 1, false);
		if(res == 1) 
			printf("Number returned: %d\n", value);
		else
			puts("ERR: no bytes_read");
		pi_gpio_toggle(LED);
		sleep_ms(1000);		
	}

	return 0;
}

