#include "hardware/gpio.h"
#include "hardware/timer.h"

#include "pi.h"

void pi_alarm_rearm(int alarm_num, uint64_t delay_us)
{
	// Clear the alarm irq
	hw_clear_bits(&timer_hw->intr, 1u << alarm_num);

	//uint32_t delay_us = 2 * 1'000'000; // 2 secs
	// Alarm is only 32 bits so if trying to delay more
	// than that need to be careful and keep track of the upper
	// bits
	uint64_t target = timer_hw->timerawl + delay_us;

	// Write the lower 32 bits of the target time to the alarm which
	// will arm it
	timer_hw->alarm[alarm_num] = (uint32_t) target;

}

void pi_alarm_init(uint alarm_num, irq_handler_t callback, uint64_t delay_us)
{
	hw_set_bits(&timer_hw->inte, 1u << alarm_num); // enable interrupt for alarm
	int irq_num = 0;
	switch(alarm_num) {
		case 0: irq_num = TIMER_IRQ_0 ; break;
		case 1: irq_num = TIMER_IRQ_1 ; break;
		case 2: irq_num = TIMER_IRQ_2 ; break;
		case 3: irq_num = TIMER_IRQ_3 ; break;
		default: assert(false);
	}
	irq_set_exclusive_handler(irq_num, callback);
        irq_set_enabled(irq_num, true);
	pi_alarm_rearm(alarm_num, delay_us);
}

void pi_gpio_init(uint gpio, pi_gpio_mode_e mode)
{
	gpio_init(gpio);
	switch(mode) {
		case INPUT_PULLUP:
			gpio_pull_up(gpio);
			[[fallthrough]];
		case INPUT:
			gpio_set_dir(gpio, GPIO_IN);
			break;
		case OUTPUT:
			gpio_set_dir(gpio, GPIO_OUT);
			break;
		default:
			assert(false);
	}
}

void pi_gpio_toggle(uint gpio)
{
	gpio_put(gpio, !gpio_get(gpio));
}
