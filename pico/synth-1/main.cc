#include <stdio.h>
#include <vector>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

using namespace std;

using  u16 = uint16_t;
using  u64 = uint64_t;

#define SPK 15

typedef struct {const char* name; u16 freq;} note_t;

const vector<note_t> notes = {
	{"C4",	262},
	{"D4",	430},
	{"E4",	834},
	{"F4",	1252},
	{"G4",	1650},
	{"A4",	2060},
	{"B4",	2480},
	{"C5",	2880},
	{"D5",	3303},
	{"E5",	3701}
	//{"F5",	
};

#define NO_NOTE_IDX  0xBAD;

bool repeating_timer_callback(struct repeating_timer *t) {
	printf("Reading\n");
	static u16 idx = NO_NOTE_IDX;
	u16 v = adc_read();
	u16 idx1 = (v+206)/411; // convert freq into note index
	if(idx == idx1) return true;
	idx=idx1;
	if(idx+1> notes.size()) {
		printf("??");
		return true;
	}
	printf("%s\n", notes[idx].name);


	return true;
}


int main() 
{
	stdio_init_all();
	printf("synth-1 started\n");
	adc_init();
	adc_gpio_init(26); // ADC0

	gpio_init(SPK);
	gpio_set_dir(SPK, GPIO_OUT);

	struct repeating_timer timer;

	//long int us = 1'000'000UL/8000/2;
	// negative means regardless of how long it took to execute
	add_repeating_timer_ms(-1000, repeating_timer_callback, NULL, &timer);


	for(;;);


	return 0;
}

