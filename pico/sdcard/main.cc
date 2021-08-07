/* File released into the Public Domain. Fill yer boots ;)
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/adc.h"
//#include "hardware/clocks.h"
//#include "hardware/flash.h"
#include "hardware/gpio.h"
//#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "hardware/spi.h"
//#include "tusb.h" // if you want to use tud_cdc_connected()

#include "fat32.h"

#include "pace.h"
//#include "../../1306/pico-sdk/oled.h"

#define SPK 19

////////////////////////////////////////////////////////////////////////////
// play sd card

uint8_t dbuf[512*2];
volatile int playing = 0, bidx = 0;
volatile signed char refill = 0; // the block that needs to be refilled


unsigned int slice_num; // determined in play_music()

void onTimer() {
	volatile static int pwm_counter = 0;
	pwm_set_gpio_level(SPK, *(dbuf + 512*playing + bidx++));
	if(bidx>=512) {
		bidx = 0;
		refill = playing;
		playing = 1-playing;
		//printf("refill = %d\n", refill
	}

	pwm_clear_irq(slice_num);
}

void play_song()
{
	char filename[] = "song.raw";
	printf("PLAY FILE: %s\n", filename);
	auto outfile{canfile(filename)};
	File file(outfile.c_str());
	if(!file.found()) {
		printf("ERR: file not found: %s\n", filename);
		return;
	}
	printf("File found. Should be good to go.\n");
	int status = pace_config_pwm_irq(&slice_num, SPK, 16000, 255, onTimer);
	if(status) printf("pwm config error\n");
	gpio_set_drive_strength(SPK, GPIO_DRIVE_STRENGTH_12MA); // boost its power output (doesn't help much)

	printf("Entering while loop\n");
	volatile unsigned char refilled = 0;
	int num_fails = 0;
	while(1) {
		volatile unsigned char _refill = refill;
		if(refilled == _refill) continue;
		file.read(dbuf + 512*_refill);
		refilled = _refill;
	}
}

////////////////////////////////////////////////////////////////////////////

void type_file(const char* filename)
{
	printf("TYPE FILE: %s\n", filename);
	auto outfile{canfile(filename)};
	File file(outfile.c_str());
	if(!file.found()) 
		printf("ERR: file not found: %s\n", filename);
	uint8_t block[512];
	while(int n = file.read(block)) {
		for(int i = 0; i< n; i++) putchar(block[i]);
	}
}

int main() 
{
	stdio_init_all();
	//while(!tud_cdc_connected()) sleep_ms(250); // wait for usb serial 
	fat32_init();
	fat32_type_partition_table();
	fat32_list_root();
	type_file("readme.txt");

	play_song();

	
	printf("sd card 5\n");

#define BTN  14 // GPIO number, not physical pin
#define LED  25 // GPIO of built-in LED
	gpio_init(BTN);
	gpio_set_dir(BTN, GPIO_IN);
	gpio_pull_up(BTN);
	// gpio_get() gets state of pin

	gpio_init(LED);
	gpio_set_dir(LED, GPIO_OUT);

	int i = 0;
	for(;;) {
		//printf("Hello number %d\n", i++);
		gpio_put(LED, 1);
		sleep_ms(100);
		gpio_put(LED, 0);
		sleep_ms(1000);		
	}

	return 0;
}

