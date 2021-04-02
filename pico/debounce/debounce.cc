#include "pico/time.h"
#include "hardware/gpio.h"

#include "debounce.h"

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

