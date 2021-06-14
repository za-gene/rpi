#pragma once

#include "hardware/irq.h"

#ifdef __cplusplus
extern "C" {
#endif



void pi_alarm_init(uint alarm_num, irq_handler_t callback, uint64_t delay_us);
void pi_alarm_rearm(int alarm_num, uint64_t delay_us);


enum pi_gpio_mode_e {INPUT, OUTPUT, INPUT_PULLUP};
void pi_gpio_init(uint gpio, pi_gpio_mode_e mode);
void pi_gpio_toggle(uint gpio);


#ifdef __cplusplus
}
#endif
