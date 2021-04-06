#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"


#define BTN 14 // GPIO number, not physical pin
#define LED 25 // GPIO of built-in LED

int main() 
{
	stdio_init_all();

	adc_init();
	adc_set_temp_sensor_enabled(true);
	adc_select_input(4);

	gpio_init(BTN);
	gpio_set_dir(BTN, GPIO_IN);
	gpio_pull_up(BTN);
	// gpio_get() gets state of pin

	gpio_init(LED);
	gpio_set_dir(LED, GPIO_OUT);

	int i = 0;
	for(;;) {
		uint16_t raw = adc_read();
		const float conversion_factor = 3.3f / (1<<12);
		float result = raw * conversion_factor;
		float temp = 27 - (result -0.706)/0.001721;
		printf("Temp = %f C\n", temp);
		//printf("Hello number %d\n", i++);
		//gpio_put(LED, 1);
		//sleep_ms(100);
		//gpio_put(LED, 0);
		sleep_ms(1000);		
	}

	return 0;
}

