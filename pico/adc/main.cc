/* loop through the ADCs printing their values in a round-robin fashion
 * User ADC inputs are on 0-3 (GPIO 26-29), the temperature sensor is on ADC 4
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"


int main() 
{
	stdio_init_all();

	adc_init();
	int i;
	for(i=0; i<4; i++) adc_gpio_init(26+i);
	adc_set_round_robin(0x1f); // all of the adcs in a round-robin fashion

again:
	for(i=0;i<5;i++) {
		//adc_select_input(i);
		uint16_t v = adc_read(); // result in range 0-4095 inc.
		printf("%5d ", (int) v);
	}
	puts("");
	sleep_ms(2000);
	goto again;

	return 0;
}

