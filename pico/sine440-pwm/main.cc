#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/adc.h"
//#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
//#include "hardware/spi.h"
// #include "tusb.h" // if you want to use tud_cdc_connected()

#include "pace.h"


using i32 = int32_t;

extern i32 sin_table[64];

// see db5.287 for calculations
const auto M = 64;
const auto dm_scale = 1 << 5; // provide fractional parts
const auto f_w = 440; //frequency of the wave we want to produce
const auto f_s = 24000; // sampling frequency; our carrier wave
constexpr i32 dm = dm_scale * M  * f_w / f_s; 

unsigned int slice_num;

#define SPK 19

void pwm_isr()
{
	pwm_clear_irq(slice_num); // reset the irq so that it will fire in future
}

int main() 
{
	stdio_init_all();
	// while(!tud_cdc_connected()) sleep_ms(250); // wait for usb serial 

#define LED  25 // GPIO of built-in LED
	gpio_init(LED);
	gpio_set_dir(LED, GPIO_OUT);
	gpio_put(LED, 1);

	int err = pace_config_pwm_irq(&slice_num, SPK, f_s, 4095, pwm_isr);
	if(err) gpio_put(LED, 0);

	for(;;);

	for(;;) {
		//printf("Hello number %d\n", i++);
		gpio_put(LED, 1);
		sleep_ms(100);
		gpio_put(LED, 0);
		sleep_ms(1000);		
	}

	return 0;
}

