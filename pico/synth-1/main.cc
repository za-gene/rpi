#include <stdio.h>
#include <vector>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

using namespace std;

using  u16 = uint16_t;
using  u32 = uint32_t;
using  u64 = uint64_t;

#define SPK 15

typedef struct {const char* name; u16 adc; u16 freq;} note_t;

const vector<note_t> notes = {
	{"C4",	11,	262},
	{"D4",	430,	294},
	{"E4",	834,	330},
	{"F4",	1252,	349},
	{"G4",	1650,	392},
	{"A4",	2060,	440},
	{"B4",	2480,	494},
	{"C5",	2880,	523},
	{"D5",	3303,	587},
	{"E5",	3701,	659},
	{"F5",	3900,	698} // F5 won't work properly
};

vector<u16> cutoffs; // for the midpoints in the notes table

//#define NO_NOTE_IDX  0xBAD;
uint slice_num; // of speaker SPK

void set_freq(u32 freq)
{
	u32 top, level;
	if(freq==0) {
		top = 0;
		level = 0;
	} else { 
		top = 1'000'000UL/freq-1;
		level = (top+1)/2-1; // 50% duty cycle
	}

	pwm_set_wrap(slice_num, top);
	pwm_set_chan_level(slice_num, pwm_gpio_to_channel(SPK), level);
}

bool repeating_timer_callback(struct repeating_timer *t) {
	static int reading_number = 0;
	//printf("Reading %d ", reading_number++);
	static int idx = -1;
	int adc = adc_read();
	//printf("adc:%d\n", adc);

	// find cutoff
	int idx1 = -1;
	for(int i=0; i< cutoffs.size(); i++) {
		if(cutoffs[i]>adc) {idx1 = i; break;}
	}
	//printf("Cutoff index: %d\n", idx1);
	if(idx == idx1) return true;
	idx=idx1;	
	u16 freq = 0;
	if(idx>=0) freq = notes[idx].freq;

	set_freq(freq);


	return true;
}


int main() 
{
	stdio_init_all();
	puts("synth-1 started");
	adc_init();
	adc_gpio_init(26); // ADC0

	gpio_set_function(SPK, GPIO_FUNC_PWM);
	slice_num = pwm_gpio_to_slice_num(SPK);
	pwm_set_clkdiv(slice_num, clock_get_hz(clk_sys)/1'000'000UL);
	pwm_set_enabled(slice_num, true);

	//init cutoffs
	puts("Init cutoffs...");
	cutoffs.reserve(notes.size());
	for(int i = 0; i< notes.size()-1; i++) {
		cutoffs.push_back((notes[i].adc + notes[i+1].adc)/2);
		printf("Cutoff %d:adc %d\n", i, cutoffs[i]);
	}
	cutoffs.push_back(3900); // not really right
	puts("...OK");

	set_freq(0);

	struct repeating_timer timer;

	u32 ms = 50;
	//ms = 1000; // for debugging purposes
	add_repeating_timer_ms(-ms, repeating_timer_callback, NULL, &timer);


	for(;;);


	return 0;
}

