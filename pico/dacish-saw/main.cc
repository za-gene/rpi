#include <stdio.h>
#include "pico/stdlib.h"
//#include "hardware/gpio.h"
//#include "hardware/clocks.h"
#include "hardware/pwm.h"

#include "pi.h"


#define ALARM 0
#define SPK 19

#define SAW // create saw wave

#ifndef SAW
#include "../mcp4921/pico-hard-song-mcp4921/data.h"
#endif

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

void alarm0_isr()
{
	pi_alarm_rearm(ALARM, delay);
#ifdef SAW
	y += dy;
#else
	static int idx = 0;
	y = data_bin[idx++]/top;
	if(idx == data_bin_len) idx = 0;
#endif
	//if(y>top) y = 0;
	if(y>1) y=0;

	// scale the level for the transistor
	const float low = 0 ? 0.9/3.3*top : 0;
	//const float low = 0;
	auto level = low + y* (top-low);
	pwm_set_gpio_level(SPK, level);
}

int main()
{
	gpio_set_function(SPK, GPIO_FUNC_PWM);
	slice_num = pwm_gpio_to_slice_num(SPK);
	pwm_set_wrap(slice_num, top);
	pwm_set_enabled(slice_num, true); // let's go!
	pi_alarm_init(ALARM, alarm0_isr, delay);

	for(;;);
	return 0;
}

