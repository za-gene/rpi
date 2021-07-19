#pragma once

#include "pico/time.h"

class Debounce {
	public:
		Debounce(uint gpio, uint delay = 4);
		bool falling();
		bool rising();
	private:
		void update();
		int _gpio;
		uint8_t _integrator = 0xFF;
		bool _falling = false;
		bool _rising = false;
		uint _delay;
		absolute_time_t later;
};

