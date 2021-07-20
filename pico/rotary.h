#pragma once

#include "pulse.h"
#include "debounce.h"

class Rotary {
	public:
		Rotary(uint sia, uint sib, uint sw);
		bool sw_falling();
		bool sw_rising();
		int change();
	private:
		uint m_sia, m_sib;
		Debounce m_sw;
		Pulse m_pulse;
		int m_prev_state = 0, m_cur_state = 0, m_count_change = 0;
		int state();
};

