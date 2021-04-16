/* iffy: blinkt doesn't seem to work with this project */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "hardware/irq.h"

#include "pace.h"
#include "../mcp4921/pico-hard-song-mcp4921/data.h"

#define	PIN_SCK		2
#define	PIN_MOSI	3
#define PIN_MISO 	4
#define	PIN_CS 		5

using u16 = uint16_t;

void my_pwm_wrap_isr();


#define LED 25
#define SPK 19

unsigned int slice_num;
const int sampling_freq = 44100;
const int top = 1023;
//const int f_pwm = 32000;

void my_pwm_wrap_isr()
{
	//const int time_scale = f_pwm / 8000;
	const int vol_scale = (top+1)/256;
	static int i = 0;
	//u16 v = data_bin[i/time_scale];
	u16 v = data_bin[i];
	i++;
	if(i == data_bin_len) i = 0;
	//a_pwm.set_level(v*vol_scale);
	int new_level = v*vol_scale;
	pwm_set_gpio_level(SPK, new_level);

	//pin16.toggle();
	pwm_clear_irq(slice_num); 
}

int main() 
{
	stdio_init_all();
	int err = pace_config_pwm_irq(&slice_num, SPK, sampling_freq, top, my_pwm_wrap_isr);
        if(err) {
            puts("Can't set PWM");
            for(;;); // loop forever, effectively halting
        }


	gpio_init(LED);
	gpio_set_dir(LED, GPIO_OUT);
	gpio_put(LED, 1);

	while(1); // actual playing of track is handled by my_pwm_wrap_isr()

	return 0;
}

