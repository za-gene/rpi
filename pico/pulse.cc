#include "pulse.h"


Pulse::Pulse(uint delay)
{
	m_delay = delay;
	m_later = make_timeout_time_ms(0);
}

bool Pulse::expired()
{
	if(absolute_time_diff_us(get_absolute_time(), m_later) > 0) 
		return false;
	m_later = make_timeout_time_ms(m_delay);
	return true;
}


