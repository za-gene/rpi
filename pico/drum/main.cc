#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "hardware/irq.h"

#include "../debounce/debounce.h"

typedef uint16_t u16;
typedef uint32_t u32;


extern unsigned char Electronic_Tom_1_raw[];
extern unsigned int Electronic_Tom_1_raw_len;

#define BTN 14 // GPIO number, not physical pin
#define LED 25 // GPIO of built-in LED
#define SPK 15

uint slice_num;


bool start = false;

void my_pwm_wrap_isr()
{
	static int i = 0;
	if(start) {
		start = false;
		i = 0;
	}

	if(i< Electronic_Tom_1_raw_len) {
		u16 v = Electronic_Tom_1_raw[i++];
		//if(i == Electronic_Tom_1_raw_len) i = 0;
		//a_pwm.set_level(v << 4);
		pwm_set_gpio_level(SPK, v);
	}

	//pin16.put(1);
	//pin16.toggle();
	//irq_clear(PWM_IRQ_WRAP);
	pwm_clear_irq(slice_num); 
}


/** NB clock divider must be in range 1.f <= value < 256.f
*/

float pwm_divider(float freq, int top)
{
	uint32_t f_sys = clock_get_hz(clk_sys); // typically 125'000'000 Hz
	float scale = (top+1) * freq;
	return (float) f_sys / scale;
}

int main() 
{
	stdio_init_all();
	slice_num = pwm_gpio_to_slice_num(SPK);

	int top = 255;
	int sampling_freq = 44'100;
	//sampling_freq = 440;
	float divider = pwm_divider(sampling_freq, top);
	pwm_set_clkdiv(slice_num, divider); // pwm clock should now be running at 1MHz

	gpio_set_function(SPK, GPIO_FUNC_PWM);
	pwm_set_wrap(slice_num, top);
	pwm_set_enabled(slice_num, true);
	pwm_set_gpio_level(SPK, top/2);

	pwm_clear_irq(slice_num);
	pwm_set_irq_enabled(slice_num, true);
	//irq_add_shared_handler(PWM_IRQ_WRAP, my_pwm_wrap_isr, PICO_SHARED_IRQ_HANDLER_DEFAULT_ORDER_PRIORITY);
	irq_set_exclusive_handler(PWM_IRQ_WRAP, my_pwm_wrap_isr);
	irq_set_enabled(PWM_IRQ_WRAP, true);

	Debounce btn(17);
	for(;;) {
		if(btn.falling()) start = true;
		//gpio_put(LED, 1);
		//sleep_ms(100);
		//gpio_put(LED, 0);
		//sleep_ms(1000);
	}

	return 0;
}

