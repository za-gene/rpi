#pragma once

#include "hardware/i2c.h"
#include "hardware/irq.h"

#ifdef __cplusplus
extern "C" {
#endif



void pi_alarm_init(uint alarm_num, irq_handler_t callback, uint64_t delay_us);
void pi_alarm_rearm(int alarm_num, uint64_t delay_us);


enum pi_gpio_mode_e {INPUT, OUTPUT, INPUT_PULLUP};
void pi_gpio_init(uint gpio, pi_gpio_mode_e mode);
void pi_gpio_toggle(uint gpio);

inline auto pi_i2c_default_port = &i2c0_inst;
void pi_i2c_init(int sda);

void pi_max7219_init(void);
void pi_max7219_show_count(int count);
void pi_max7219_tfr(uint8_t address, uint8_t value);

void pi_spi_init_std(void);

#ifdef __cplusplus
}
#endif
