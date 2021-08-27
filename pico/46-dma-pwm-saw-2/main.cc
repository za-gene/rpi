#include <stdio.h>
#include <string.h>
//#define PARAM_ASSERTIONS_ENABLE_ALL 1
#include "pico/stdlib.h"
#include <assert.h>
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "hardware/dma.h"
// #include "tusb.h" // if you want to use tud_cdc_connected()

//#include "pi.h"
//#include "pace.h"

#define SPK 19

#define TOP 255
#define NSAMPS 256
unsigned int slice_num;
int dma_chan;

#define pinfo(x) printf("%s = %u\n", #x, x);

volatile uint16_t val = 0;

void dma_handler()
{
	//dma_irqn_acknowledge_channel(??, dma_chan);
	dma_hw->ints0=1u<<dma_chan; // clear the interrupt request
	dma_channel_transfer_from_buffer_now(dma_chan, &val, 1);
	val++;
	if(val == 256) val = 0;

}

int main() 
{
	stdio_init_all();
	// while(!tud_cdc_connected()) sleep_ms(250); // wait for usb serial 
	puts("\nsawtooth wave via dma and pwm");

	const int pwm_chan = pwm_gpio_to_channel(SPK);
	pinfo(pwm_chan); 

	// set up pwm
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
	channel_config_set_transfer_data_size(&cfg, DMA_SIZE_16);
	channel_config_set_read_increment(&cfg, true);
	channel_config_set_dreq(&cfg, DREQ_PWM_WRAP0 + slice_num); // write data at pwm frequency
	//channel_config_set_write_increment(&cfg, false);
	//uint32_t write_address = (uint32_t) &pwm_hw->slice[slice_num].cc; // base address
	//if(pwm_chan) write_address += 2; // adjust for channel
	//write_address +=1;
	dma_channel_configure(
		dma_chan,          // Channel to be configured
		&cfg,            // The configuration we just created
		//(volatile void*) write_address,           // The initial write address
		&pwm_hw->slice[slice_num].cc, // write address
		&val,           // The initial read address
		1, // Number of transfers
		false           // Start immediately?
		);

	dma_channel_set_irq0_enabled(dma_chan,true);
	//irq_set_exclusive_handler(DMA_IRQ_0,dma_handler);
	irq_add_shared_handler(DMA_IRQ_0, dma_handler, 0x20); // set its priority fairly high
	irq_set_enabled(DMA_IRQ_0,true);
	dma_handler(); // kick off
	//dma_channel_start(dma_chan); // helps?

	while(1) {
		tight_loop_contents();
		//dma_channel_transfer_from_buffer_now(dma_chan, table, NSAMPS);
		//dma_channel_wait_for_finish_blocking(dma_chan);
		//dma_channel_start(dma_chan);
	}
	return 0;
}

