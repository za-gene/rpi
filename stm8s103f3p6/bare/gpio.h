#pragma once
#include <stm8.h>

#define PD4 1
#define PD5 2
#define PD6 3
#define PA1 5
#define PA2 6
#define PA3 10
#define PB5 11
#define PB4 12
#define PC3 13
#define PC4 14 
#define PC5 15
#define PC6 16
#define PC7 17 
#define PD1 18 
#define PD2 19 
#define PD3 20

#define BUILTIN_LED PB5

PORT_t* pin_to_port(u8 pin);
u8 pin_to_pos(u8 pin);
void gpio_mode_in(u8 pin);
void gpio_mode_out(u8 pin);
void gpio_mode_pullup(u8 pin);
void gpio_toggle(u8 pin);
u8 gpio_read(u8 pin);
void gpio_write(u8 pin, u8 val);
