#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/spi.h"
#include "hardware/gpio.h"


//#define BTN 14 // GPIO number, not physical pin
#define LED 25 // GPIO of built-in LED

void toggle()
{
	gpio_xor_mask(1<<LED); // toggle LED
}

#if 0
#define NUM_PINS 30 // GPIO pins are 0-29 inc.. There are no GPIO30 or GPIO31

#define FOR_PINS for(int i = 0; i< NUM_PINS; i++)

static uint8_t integrators[NUM_PINS];
static uint32_t gpio_mask = 0; // the pins we are interested in debouncing
static uint32_t falling_mask = 0;
static uint32_t rising_mask = 0;

static bool repeating_timer_callback(struct repeating_timer *t)
{
	uint32_t mask = gpio_mask;
	uint32_t values = gpio_get_all();
	FOR_PINS {
		if(mask & 1) {
			int old_val = integrators[i];
			integrators[i] <<= 1;
			integrators[i] |= (values & 1);
			if(old_val != 0xFF && integrators[i] == 0xFF) {
				//gpio_put(LED, 1);
				rising_mask |= (1<<i);
				//falling_mask |= (1<<i);
			}
			if(old_val != 0 && integrators[i] == 0) {
				//gpio_put(LED, 1);
				falling_mask |= (1<<i);
			}

		}
		mask >>= 1;
		values >>= 1;
	}

	return true;
}

static struct repeating_timer timer;

void debounce_gpio(uint gpio)
{
	static bool initialised = false;
	if(!initialised) {
		initialised = true;
		add_repeating_timer_ms(3, repeating_timer_callback, NULL, &timer);
		FOR_PINS integrators[i] = 0xFF;
	}

	gpio_init(gpio);
	gpio_set_dir(gpio, GPIO_IN);
	gpio_pull_up(gpio);
	gpio_mask |= (1<<gpio);
}

bool debounce_falling(uint gpio)
{
	bool yes = falling_mask & (1<<gpio);
	falling_mask &= ~(1<<gpio);
	return yes;
}

bool debounce_rising(uint gpio)
{
	bool yes = rising_mask & (1<<gpio);
	rising_mask &= ~(1<<gpio);
	return yes;
}
#endif

class Debounce {
	public:
		Debounce(uint gpio, uint delay = 4);
		bool falling();
		bool rising();
	private:
		void update();
		uint _gpio;
		uint8_t _integrator = 0xFF;
		bool _falling = false;
		bool _rising = false;
		uint _delay;
};

Debounce::Debounce(uint gpio, uint delay)
{
	gpio_init(gpio);
	gpio_set_dir(gpio, GPIO_IN);
	gpio_pull_up(gpio);
	_gpio = gpio;
	_delay = delay;
}

void Debounce::update()
{
	static absolute_time_t later = make_timeout_time_ms(0);
	if(absolute_time_diff_us(get_absolute_time(), later)>0) return;
	later = make_timeout_time_ms(_delay);
	uint8_t prev = _integrator;
	uint8_t up = gpio_get(_gpio) ? 0 : 1;
	_integrator <<= 1;
	_integrator |= up;
	if(prev != 0xFF && _integrator == 0xFF) _falling = true;
	if(prev != 0 && _integrator == 0) _rising = true;
}

bool Debounce::falling()
{
	update();
	if(_falling) {
		_falling = false;
		return true;
	}
	return false;
}

bool Debounce::rising()
{
	update();
	if(_rising) {
		_rising = false;
		return true;
	}
	return false;
}

int main() 
{
	stdio_init_all();

	Debounce button(17);

	gpio_init(LED);
	gpio_set_dir(LED, GPIO_OUT);

	for(;;) {
		if(button.falling()) toggle();
		//if(button.rising()) toggle();
	}

	return 0;
}

