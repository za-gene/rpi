#include <stdio.h>
#include <string.h>
//#define PARAM_ASSERTIONS_ENABLE_ALL 1
#include "pico/stdlib.h"
#include <assert.h>
#include "hardware/clocks.h"
//#include "hardware/flash.h"
#include "hardware/gpio.h"
//#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "hardware/dma.h"
// #include "tusb.h" // if you want to use tud_cdc_connected()

//#include "pi.h"
//#include "pace.h"

#define SPK 19

#define TOP 255
#define NSAMPS 256
uint32_t table[NSAMPS];
unsigned int slice_num;
int dma_chan;

#define pinfo(x) printf("%s = %u\n", #x, x);

int main() 
{
	stdio_init_all();
	// while(!tud_cdc_connected()) sleep_ms(250); // wait for usb serial 
	puts("\nsawtooth wave via dma and pwm");

	// fill in table
	const int pwm_chan = pwm_gpio_to_channel(SPK);
	//const int pwm_channel_shift = pwm_chan == 0 ?  0 : 16;
	for(int i = 0; i< NSAMPS; i++) {
		table[i] = i << (pwm_chan ? PWM_CH0_CC_B_LSB : PWM_CH0_CC_A_LSB);
	}
	printf("channel is: %d\n", pwm_chan); 

	// set up pwm
	pinfo(count_of(table));
	pinfo(PWM_CH0_CC_A_LSB);
	pinfo(PWM_CH0_CC_B_LSB);
	const int freq = 500 * NSAMPS; // 500Hz sawtooth
	//int status = pace_config_pwm(&slice_num, SPK, freq,255);
	gpio_set_function(SPK, GPIO_FUNC_PWM);
	slice_num = pwm_gpio_to_slice_num(SPK);
	float divider = (float) clock_get_hz(clk_sys) /(TOP+1)/freq;
	printf("divider=%f\n", divider); // comes out at 3
	pwm_set_clkdiv(slice_num, divider);
	pwm_set_wrap(slice_num, TOP);
	pwm_set_enabled(slice_num, true);
	//pwm_set_enabled(slice_num, false); // does disabling help? No.
	pinfo(slice_num); // apparently SPK 19 is slice 1
	//printf("GPIO on consistent channel?: %s\n", pwm_gpio_to_channel(SPK)  == 0 ? "GOOD" : "BAD");
	pwm_set_irq_enabled(slice_num, true); // Necessary? Yes
	//irq_set_enabled(PWM_IRQ_WRAP, true);// Necessary? Seems to cause problem

	dma_chan = dma_claim_unused_channel(true);
	dma_channel_config cfg = dma_channel_get_default_config(dma_chan);
	channel_config_set_transfer_data_size(&cfg, DMA_SIZE_32);
	channel_config_set_read_increment(&cfg, true);
	channel_config_set_dreq(&cfg, DREQ_PWM_WRAP0 + slice_num); // write data at pwm frequency
	//channel_config_set_write_increment(&cfg, false);
	dma_channel_configure(
			dma_chan,          // Channel to be configured
			&cfg,            // The configuration we just created
			&pwm_hw->slice[slice_num].cc,           // The initial write address
			table,           // The initial read address
			NSAMPS, // Number of transfers
			false           // Start immediately?
			);
	//dma_channel_start(dma_chan); // didn't help

	while(1) {
		dma_channel_transfer_from_buffer_now(dma_chan, table, NSAMPS);
		dma_channel_wait_for_finish_blocking(dma_chan);
		//dma_channel_start(dma_chan);
	}
	return 0;
}

