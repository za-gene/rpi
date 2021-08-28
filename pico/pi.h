#pragma once

#include "hardware/i2c.h"
#include "hardware/irq.h"

#ifdef __cplusplus
extern "C" {
#endif



void pi_alarm_init(uint alarm_num, irq_handler_t callback, uint32_t delay_us);

// declaring it static inline seems to fix the fluttering problems
static inline void pi_alarm_rearm(int alarm_num, uint32_t delay_us)
{
	// Clear the alarm irq
	hw_clear_bits(&timer_hw->intr, 1u << alarm_num);

	//uint32_t delay_us = 2 * 1'000'000; // 2 secs
	// Alarm is only 32 bits so if trying to delay more
	// than that need to be careful and keep track of the upper
	// bits
	//uint64_t target = timer_hw->timerawl + delay_us;

	// Write the lower 32 bits of the target time to the alarm which
	// will arm it
	//timer_hw->alarm[alarm_num] = (uint32_t) target;
	
	timer_hw->alarm[alarm_num] = timer_hw->timerawl + delay_us;

}


typedef enum {INPUT, OUTPUT, INPUT_PULLUP} pi_gpio_mode_e;
void pi_gpio_init(uint gpio, pi_gpio_mode_e mode);
void pi_gpio_high(uint gpio);
void pi_gpio_low(uint gpio);
void pi_gpio_out(uint gpio);
int pi_gpio_is_high(uint gpio);
void pi_gpio_toggle(uint gpio);

extern i2c_inst_t *pi_i2c_default_port;
void pi_i2c_init(int sda);

void pi_max7219_init(void);
void pi_max7219_show_count(int count);
void pi_max7219_tfr(uint8_t address, uint8_t value);

void pi_spi_init_std(void);

#ifdef __cplusplus
}


class GpioOut {
	public:
		GpioOut(uint gpio);
		void on(void);
		void off(void);
	private:
		uint m_gpio;
};

#endif // __cplusplus
