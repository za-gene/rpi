#pragma once

#include "pico/time.h"

class Pulse {
	public:
		Pulse(uint delay = 1);
		bool expired();
	private:
		uint m_delay;
		absolute_time_t m_later;
};


