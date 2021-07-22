/* generates sawtooth wave
 */

#include <algorithm>
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/adc.h"
//#include "hardware/clocks.h"
//#include "hardware/flash.h"
#include "hardware/gpio.h"
//#include "hardware/irq.h"
#include "hardware/pwm.h"
//#include "hardware/spi.h"
// #include "tusb.h" // if you want to use tud_cdc_connected()

#include "notes.h"
#include "pi.h"
#include "rotary.h"

#define ALARM 	0
#define SPK	15
typedef uint16_t u16;
typedef uint32_t u32;

auto top = 255.0;
auto wave_freq = 440.0;
auto framerate = 44100.0;
auto delay = 1'000'000.0/framerate;
uint slice_num;
float y = 0;
//float dy = top * wave_freq / framerate;
float dy = wave_freq / framerate;


int notes_find(const char* note_name)
{
	for(int i = 0; i<num_notes; i++) {
		if(strcmp(note_name, notes[i].name)==0)
			return i;
	}
	return -1;
}




auto notes_idx = notes_find("C4");



void set_freq()
{
	wave_freq = notes[notes_idx].freq;
	dy = wave_freq / framerate;
}

void alarm0_isr()
{
	pi_alarm_rearm(ALARM, delay);
	y += dy;
	if(y>1) y=0;

	// scale the level for the transistor
	//const float low = 0 ? 0.9/3.3*top : 0;
	const float low = 0;
	auto level = low + y* (top-low);
	pwm_set_gpio_level(SPK, level);
}


int main() 
{
	stdio_init_all();
	// while(!tud_cdc_connected()) sleep_ms(250); // wait for usb serial 

#define BTN  14 // GPIO number, not physical pin
#define LED  25 // GPIO of built-in LED
	gpio_init(BTN);
	gpio_set_dir(BTN, GPIO_IN);
	gpio_pull_up(BTN);
	// gpio_get() gets state of pin

	gpio_init(LED);
	gpio_set_dir(LED, GPIO_OUT);

	gpio_set_function(SPK, GPIO_FUNC_PWM);
	slice_num = pwm_gpio_to_slice_num(SPK);
	pwm_set_wrap(slice_num, top);
	pwm_set_enabled(slice_num, true); // let's go!
	pi_alarm_init(ALARM, alarm0_isr, delay);

	Rotary rot(21, 20, 19);

	set_freq();

	for(;;) {
		if(int chg = rot.change()) {
			auto new_idx = std::clamp(notes_idx + chg, 0, num_notes-1);
			if(new_idx != notes_idx) {
				notes_idx = new_idx;
				set_freq();
			}

		}
#if 0
		printf("Hello number %d\n", i++);
		gpio_put(LED, 1);
		sleep_ms(100);
		gpio_put(LED, 0);
		sleep_ms(1000);		
#endif
	}

	return 0;
}

