#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
//#include "hardware/spi.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "tusb.h"


#define LED 25 // GPIO of built-in LED

/*
typedef struct {
	uint slice_num;

} pace_pwm_irq_t;

pace_pwm_irq_t pace_pwm_irq;
*/


uint slice_num;
uint speaker = 14;

void my_wrap_isr()
{
	static uint32_t count = 0;
	gpio_put(LED, 1);
	uint level = (count * 32000/440) % 256;
	pwm_set_gpio_level(speaker, level);
	count++;
	pwm_clear_irq(slice_num);
}

/** NB clock divider must be in range 1.f <= value < 256.f
*/

float pace_pwm_divider(int freq, int top)
{
        uint32_t f_sys = clock_get_hz(clk_sys); // typically 125'000'000 Hz
        float scale = (top+1) * freq;
        return f_sys / scale;
}

int pace_config_pwm_irq(uint* slice_num, uint gpio, int freq, int top, irq_handler_t pwm_irq_wrap_handler)
{
	//int result = 0; // indicates success
	// no need for gpio_init() or gpio_set_dir(), just use:
	gpio_set_function(gpio, GPIO_FUNC_PWM); 

	*slice_num = pwm_gpio_to_slice_num(gpio);
	float divider = pace_pwm_divider(freq, top);
	if(divider < 1.0 || divider >= 256.0) return 1;
	//printf("Divider: %f\n", (double) divider);
	//if( 1.0 <= divider && divider < 256.0) gpio_put(LED,1);
	pwm_set_clkdiv(*slice_num, divider);
	pwm_set_wrap(*slice_num, top);
	pwm_set_enabled(*slice_num, true);

	pwm_clear_irq(*slice_num);
	pwm_set_enabled(*slice_num, true);
	//irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_irq_wrap_handler);
	irq_set_exclusive_handler(PWM_IRQ_WRAP, my_wrap_isr);
	irq_set_enabled(PWM_IRQ_WRAP, true);

	return 0;
}



int main() 
{
	stdio_init_all();
	//while(!tud_cdc_connected()) sleep_ms(250);

	//for(;;);

	gpio_init(LED);
	gpio_set_dir(LED, GPIO_OUT);
	int res = pace_config_pwm_irq(&slice_num, speaker, 24000, 255, my_wrap_isr);
	if(res) {
		puts("Error in setup of pace_config_pwm_irq()");
		for(;;); // just hang
	}
	gpio_put(LED, 1);



	for(;;);

	int i = 0;
	for(;;) {
		printf("Hello number %d\n", i++);
		gpio_put(LED, 1);
		sleep_ms(100);
		gpio_put(LED, 0);
		sleep_ms(1000);		
	}

	return 0;
}

