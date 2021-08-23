#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#include "pace.h"


/** NB clock divider must be in range 1.f <= value < 256.f
*/

float pace_pwm_divider(int freq, int top)
{
	uint32_t f_sys = clock_get_hz(clk_sys); // typically 125'000'000 Hz
	float scale = (top+1) * freq;
	return f_sys / scale;
}

int pace_config_pwm(unsigned int* slice_num, unsigned int gpio, int freq, int top)
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
	return 0;
}

int pace_config_pwm_irq(unsigned int* slice_num, unsigned int gpio, int freq, int top, 
		irq_handler_t pwm_irq_wrap_handler)
{
	if(pace_config_pwm(slice_num, gpio, freq, top)) return 1;
	pwm_clear_irq(*slice_num);
	pwm_set_irq_enabled(*slice_num, true);
	irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_irq_wrap_handler);
	//irq_set_exclusive_handler(PWM_IRQ_WRAP, my_wrap_isr);
	irq_set_enabled(PWM_IRQ_WRAP, true);

	return 0;
}

