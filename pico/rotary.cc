#include "rotary.h"
#include "pi.h"


Rotary::Rotary(uint sia, uint sib, uint sw) : m_sia(sia), m_sib(sib), m_sw(Debounce(sw))
{
	pi_gpio_init(sia, INPUT);
	pi_gpio_init(sib, INPUT);

}


bool Rotary::sw_falling() { return m_sw.falling(); }
bool Rotary::sw_rising() { return m_sw.rising(); }

int Rotary::state()
{
	return (pi_gpio_is_high(m_sib) <<1) | pi_gpio_is_high(m_sia); 
}

int Rotary::change()
{
	if(m_pulse.expired()) {
		m_cur_state = state();
		if((m_prev_state == 2) && (m_cur_state == 3)) {
			m_count_change++;
			//puts("increase");
		}
		if((m_prev_state == 1) && (m_cur_state == 3)) {
			m_count_change--;
		}
		//if(count != prev_count)
		//	printf("count %d = %d\n", ++count_num, count);
		m_prev_state = m_cur_state;
	}
	auto tmp = m_count_change;
	m_count_change = 0;
	return tmp;

}

