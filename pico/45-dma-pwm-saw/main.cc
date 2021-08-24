#include <stdio.h>
#include <string.h>
//#define PARAM_ASSERTIONS_ENABLE_ALL 1
#include "pico/stdlib.h"
#include <assert.h>
//#include "hardware/clocks.h"
//#include "hardware/flash.h"
#include "hardware/gpio.h"
//#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "hardware/dma.h"
// #include "tusb.h" // if you want to use tud_cdc_connected()

#include "pi.h"
#include "pace.h"

#define SPK 19

uint32_t table[256];
unsigned int slice_num;
int dma_chan;

int main() 
{
	stdio_init_all();
	// while(!tud_cdc_connected()) sleep_ms(250); // wait for usb serial 
	puts("\nsawtooth wave via dma and pwm");

	// fill in table
	const int pwm_chan = pwm_gpio_to_channel(SPK);
	const int pwm_channel_shift = pwm_chan == 0 ?  0 : 16;
	for(int i = 0; i< 256; i++) {
		table[i] = (i << pwm_channel_shift);
	}
	printf("channel is: %d\n", pwm_chan); 

	// set up pwm
	printf("count_of(table)=%d\n", count_of(table));
	const int freq = 500 * count_of(table); // 500Hz sawtooth
	int status = pace_config_pwm(&slice_num, SPK, freq,255);
	//pwm_set_enabled(slice_num, false); // does disabling help? No.
	printf("slice num:%d\n", slice_num); // apparently SPK 19 is slice 1
	//printf("GPIO on consistent channel?: %s\n", pwm_gpio_to_channel(SPK)  == 0 ? "GOOD" : "BAD");
	pwm_set_irq_enabled(slice_num, true); // Necessary? Yes
	//irq_set_enabled(PWM_IRQ_WRAP, true);// Necessary? Seems to cause problem

	dma_chan = dma_claim_unused_channel(true);
	dma_channel_config cfg = dma_channel_get_default_config(dma_chan);
	channel_config_set_transfer_data_size(&cfg, DMA_SIZE_32);
	channel_config_set_read_increment(&cfg, true);
	//channel_config_set_write_increment(&cfg, false);
	dma_channel_configure(
			dma_chan,          // Channel to be configured
			&cfg,            // The configuration we just created
			&pwm_hw->slice[slice_num].cc,           // The initial write address
			table,           // The initial read address
			256, // Number of transfers
			false           // Start immediately?
			);
	channel_config_set_dreq(&cfg, DREQ_PWM_WRAP0 + slice_num); // write data at pwm frequency
	//dma_channel_start(dma_chan); // didn't help

	while(1) {
		dma_channel_transfer_from_buffer_now(dma_chan, table, 256);
		dma_channel_wait_for_finish_blocking(dma_chan);
		//dma_channel_start(dma_chan);
	}


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

