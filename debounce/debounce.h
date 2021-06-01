#pragma once

#include <stdint.h>
#if 1
class Debounce {
	public:
		Debounce(int gpio, int delay = 6);
		bool falling();
		bool rising();
	private:
		void update();
		int _gpio;
		uint8_t _integrator = 0xFF;
		bool _falling = false;
		bool _rising = false;
		int _delay;
        unsigned long _later;
};

#else
class FallingButton {

	public:
		FallingButton(int pin);
		bool falling();

	private:
		int _pin;
		char zone; // 0 = high state, 1 = falling, 2 = low state, 3 = rising
		bool low();
		bool high();
		unsigned long started;
};
#endif
