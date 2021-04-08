#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "hardware/irq.h"

#include "../debounce/debounce.h"

typedef uint16_t u16;
typedef uint32_t u32;


extern unsigned char Electronic_Tom_1_raw[];
extern unsigned int Electronic_Tom_1_raw_len;

Debounce btn(17);
#define SPK 18


#define TRIG_1 19
#define TRIG_2 20

bool start = false;

void trigger_callback(uint gpio, uint32_t events)
{
	start = true;
}

void trigger_init(uint gpio, gpio_irq_callback_t callback)
{
	//uint gpio = TRIG;
	//static auto callback = [](uint gpio, uint32_t events) { start = true;  };
	gpio_init(gpio);
	gpio_set_dir(gpio, GPIO_IN);
	gpio_set_irq_enabled_with_callback(gpio, GPIO_IRQ_EDGE_RISE, true, callback);

}

uint slice_num;



void my_pwm_wrap_isr()
{
	static int i = -1; // init to -1 so that we don't trigger the sample at bootup
	if(start) {
		start = false;
		i = 0;
	}

	if(-1 < i && i< Electronic_Tom_1_raw_len) {
		u16 v = Electronic_Tom_1_raw[i++];
		pwm_set_gpio_level(SPK, v);
	}

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
	slice_num = pwm_gpio_to_slice_num(SPK);

	int top = 255;
	int sampling_freq = 44'100;
	//sampling_freq = 440;
	float divider = pwm_divider(sampling_freq, top);
	pwm_set_clkdiv(slice_num, divider);

	gpio_set_function(SPK, GPIO_FUNC_PWM);
	pwm_set_wrap(slice_num, top);
	pwm_set_enabled(slice_num, true);
	pwm_set_gpio_level(SPK, 0);

	pwm_clear_irq(slice_num);
	pwm_set_irq_enabled(slice_num, true);
	//irq_add_shared_handler(PWM_IRQ_WRAP, my_pwm_wrap_isr, PICO_SHARED_IRQ_HANDLER_DEFAULT_ORDER_PRIORITY);
	irq_set_exclusive_handler(PWM_IRQ_WRAP, my_pwm_wrap_isr);
	irq_set_enabled(PWM_IRQ_WRAP, true);

	trigger_init(TRIG_1, trigger_callback);
	trigger_init(TRIG_2, trigger_callback);

	for(;;) {
		if(btn.falling()) start = true;
	}

	return 0;
}

