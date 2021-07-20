#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/adc.h"
//#include "hardware/clocks.h"
//#include "hardware/flash.h"
#include "hardware/gpio.h"
//#include "hardware/irq.h"
//#include "hardware/pwm.h"
//#include "hardware/spi.h"
// #include "tusb.h" // if you want to use tud_cdc_connected()
#include <inttypes.h>

#include "pi.h"
#include "debounce.h"
#include "pulse.h"

#define ALARM 0
#define DELAY 1'000


#define SIA 	21
//Debounce sia(SIA, 1);
#define SIB 	20
#define SW	19 
//Debounce sib(SIB, 1);
//#define BTN  22 // GPIO number, not physical pin
#define LED  25 // GPIO of built-in LED

volatile uint32_t count = 0;


volatile uint32_t ev_num = 0;


int pi_gpio_is_high(uint gpio)
{
	if(gpio_get(gpio) == 0) return 0;
	return 1;
}

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

#if 0
static void millis_irq()
{
	pi_alarm_rearm(ALARM, DELAY);
	volatile static uint64_t millis = 0;
	//volatile static uint8_t prev_grate = gpio_is_high(SIA) * 0xFF;
	//volatile static uint8_t cur_grate= prev_grate;
	volatile static int i = 0;
	volatile static int count = 0;
	static auto state = [](){return (gpio_is_high(SIB) <<1) | gpio_is_high(SIA); };
	volatile static int prev_state = state();
	if(millis % 1 == 0) {
		int cur_state = state();
		int prev_count = count;
		if(cur_state != prev_state) {
			//printf("cur_state %d (ms) = %d, prev = %d\n", i++, cur_state, prev_state);
			if((prev_state == 2) && (cur_state == 3)) {
				count ++;
				//puts("increase");
			}
			if((prev_state == 1) && (cur_state == 3)) {
				count--;
			}
			if(count != prev_count)
				printf("count %d = %d\n", i++, count);
			prev_state = cur_state;
		}
	}

#if 0
	if(millis % 500 == 0) {
		//puts("hi");
		pi_gpio_toggle(LED);
	}
#endif
	millis++;

}
#endif

int main() 
{
	stdio_init_all();
	// while(!tud_cdc_connected()) sleep_ms(250); // wait for usb serial 

	Rotary rot(SIA, SIB, SW);
	pi_gpio_init(LED, OUTPUT);
	//pi_alarm_init(ALARM, millis_irq, DELAY);

	Pulse pulse(500);
	int count = 0;
	int count_num = 0;
	while(1) {
		if(rot.sw_falling()) {
			puts("Rotary button pushed");
		}
		if(pulse.expired())
			pi_gpio_toggle(LED);
		if(int chg = rot.change()) {
			count += chg;
			printf("count %d = %d\n", ++count_num, count);
		}
	}

	return 0;
}

