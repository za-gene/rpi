#include "debounce.h"
#include "Arduino.h"

#if 1
Debounce::Debounce(int gpio, int delay)
{
    pinMode(gpio, INPUT_PULLUP);
	_gpio = gpio;
	_delay = delay;
}

void Debounce::update()
{
	//static absolute_time_t later = make_timeout_time_ms(0);
    static auto later = millis();
	//if(absolute_time_diff_us(get_absolute_time(), later)>0) return;
    if(millis() - later > _delay) return;
	//later = make_timeout_time_ms(_delay);
    later = millis() + _delay;
	uint8_t prev = _integrator;
	uint8_t up = digitalRead(_gpio) ? 0 : 1;
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

#else
FallingButton::FallingButton(int pin): _pin(pin) {
	pinMode(pin, INPUT_PULLUP);
}

bool FallingButton::falling() {

	bool expired = (millis() - started) > 50;

	switch (zone) {
		case 0: // high, looking for falling
			if (high()) break;
			started = millis();
			zone = 1;
			return true;
		case 1: // in a falling region. Ignore for awhile
			if (expired) zone = 2;
			break;
		case 2: // low state region, check for release
			if (low()) break;
			started = millis();
			zone = 3;
			break;
		case 3: // in a rising region. Ignore for awhile
			if (expired) zone = 0;
			break;
	}
	return false;
}

bool FallingButton::low() {
	return digitalRead(_pin) == LOW;
}
bool FallingButton::high() {
	return digitalRead(_pin) == HIGH;
}
#endif
