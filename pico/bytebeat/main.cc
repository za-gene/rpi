/*
#include <string.h>
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "tusb.h"
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#include "pace.h"

#define LED 25 // GPIO of built-in LED


uint slice_num;
uint speaker = 14;

const int freq_scale = 3; // we actually want a frequency of 8kHz, but scale it up to remove a hum

void my_wrap_isr()
{
	static uint32_t count = 0; // raw count
	static uint32_t t = 0; // a time count in terms of 8kHz
	if(count % freq_scale == 0) { // we had sped up the carrier frequency to remove the hum

		// this is where the magic happens. Adjust the level 
		// according to your own bytebeat algorithm
		uint32_t level = t * (42&t>>10);

		pwm_set_gpio_level(speaker, (uint8_t) level);
		t++;
	}
	count++;
	pwm_clear_irq(slice_num); // reset the irq so that it will fire in future
}



int main() 
{
	stdio_init_all();
	//while(!tud_cdc_connected()) sleep_ms(250);

	gpio_init(LED);
	gpio_set_dir(LED, GPIO_OUT);
	int err = -1;

	err = pace_config_pwm_irq(&slice_num, speaker, 8000* freq_scale, 255, my_wrap_isr);
	//err = pace_config_pwm_irq(&slice_num, speaker, 880, 4095, my_wrap_isr);
	if(err) {
		puts("Error in setup of pace_config_pwm_irq()");
		for(;;); // just hang
	}
	gpio_put(LED, 1);

	for(;;);

	return 0;
}

