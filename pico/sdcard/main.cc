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
#include "mcp4921-dma.h"
#include "pace.h"
#include "pi.h"
//#include "../../1306/pico-sdk/oled.h"

#if 1
#define SPK 19
#else
#define SPK 7
#endif

////////////////////////////////////////////////////////////////////////////
// play sd card



unsigned int slice_num; // determined in play_music()
//constexpr auto isr_multiplier = 1; // speed-up the timer to avoid audible clicks. doesn't help, though.

const bool use_pwm = 1;

#define ALARM 0
#define DELAY (1'000'000/16'000)


uint8_t dbuf[512*2];
volatile int dbuf_offset = -1; // either -1: don't do anything; 0: offset 0; 512: offset 512

void song_buffer_poll(file32_t *file)
{
	if(dbuf_offset == -1) return;
	//volatile unsigned char _refill = refill;
	//if(refilled == _refill) continue;
	auto dst = dbuf + dbuf_offset;
	memset(dst, 0, 512);
	int n = file32_read(file, dst);
	if(n<512) {		
		file32_seek0(file); // repeat the song
	}
	//refilled = _refill;
	dbuf_offset = -1;
}

uint8_t get_vol()
{
	volatile static int playing = 0, bidx = 0;
	//volatile signed char refill = 0; // the block that needs to be refilled
	uint8_t vol = *(dbuf + 512*playing + bidx++);
	if(bidx>=512) {
		bidx = 0;
		dbuf_offset = 512*playing;
		//refill = playing;
		playing = 1-playing;
	}
	return vol;
}

void sound_set_level()
{
	pi_alarm_rearm(ALARM, DELAY);
	//uint8_t vol = *(dbuf + 512*playing + bidx++);
	uint8_t vol = get_vol();
	if(use_pwm) {
		pwm_set_gpio_level(SPK, vol);
	} else {
		uint16_t vol16 = ((uint16_t) vol ) << 4;
		mcp4921_dma_put(vol16);
	}

}

void sound_init(void)
{
	pi_alarm_init(ALARM, sound_set_level, DELAY);
	//irq_set_priority(PWM_IRQ_WRAP, PICO_HIGHEST_IRQ_PRIORITY); // doesn't seem to help
	if(use_pwm)
		pace_config_pwm(&slice_num, SPK, 16000, 255);
	else
		mcp4921_dma_init();
}





void play_song()
{
	char filename[] = "song.raw";
	printf("PLAY FILE: %s\n", filename);
	file32_t file;
	file32_init(&file, filename);
	if(!file32_found(&file)) {
		printf("ERR: file not found: %s\n", filename);
		return;
	}
	printf("File found. Should be good to go.\n");
	sound_init();

	printf("Entering while loop\n");
	//volatile unsigned char refilled = 0;
	int num_fails = 0;
	while(1) {
		song_buffer_poll(&file);
	}
}


////////////////////////////////////////////////////////////////////////////


void type_file(const char* filename)
{
	printf("TYPE FILE: %s\n", filename);
	file32_t file;
	file32_init(&file, filename);
	if(!file32_found(&file)) 
		printf("ERR: file not found: %s\n", filename);
	uint8_t block[512];
	while(int n = file32_read(&file, block)) {
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

