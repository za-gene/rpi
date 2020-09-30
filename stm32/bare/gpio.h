#pragma once
#include "blue.h"

// http://embedded-lab.com/blog/stm32-gpio-ports-insights/

/* A more refactored style for the registers can be found at
 * https://github.com/WRansohoff/STM32x0_timer_example/blob/master/device_headers/stm32f031x6.h
 */

// register address
//#define RCC_BASE      0x40021000


//#define RCC_APB2ENR   *(volatile uint32_t *)(RCC_BASE   + 0x18)


// pins are defined as hex 0xPQ where P is port, and Q is pin number
// Currently incomplete


#define PA4  0x04
#define PA5  0x05
#define PA7  0x07
#define PA13 0x0D
#define PB0  0x10
#define PB1  0x11
#define PB13 0x1D
#define PC13 0x2D
#define PC15 0x2F

#define BUILTIN_LED PC13


//u32 pin_to_gpio(u32 pin);
void gpio_write(u32 pin, u32 val);
void gpio_mode_out(u32 pin);
void gpio_toggle(u32 pin);
