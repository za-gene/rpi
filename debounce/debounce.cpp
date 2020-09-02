#include "debounce.h"
#include "Arduino.h"


FallingButton::FallingButton(int pin): _pin(pin) {
	pinMode(pin, INPUT_PULLUP);
}

bool FallingButton::falling() {

	bool expired = (millis() - started) > 25;

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
