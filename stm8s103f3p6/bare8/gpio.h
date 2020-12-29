#pragma once
#include <stm8.h>

#define PA1 0x01
#define PA2 0x02
#define PA3 0x03

#define PB4 0x14
#define PB5 0x15

#define PC3 0x23
#define PC4 0x24
#define PC5 0x25
#define PC6 0x26
#define PC7 0x27 

#define PD1 0x31
#define PD2 0x32 
#define PD3 0x33
#define PD4 0x34
#define PD5 0x35
#define PD6 0x36

#define LED_BUILTIN PB5

#define INPUT 0
#define INPUT_PULLUP 1
#define OUTPUT 2

#define LOW 0
#define HIGH 1

void digitalWrite(u8 pin, u8 value);
void digitalToggle(u8 pin);
u8 digitalRead(u8 pin);
void pinMode(u8 pin, u8 value);


typedef struct {
	uchar ODR;
	uchar IDR;
	uchar DDR;
	uchar CR1;
	uchar CR2;
} PORT_t;


/*
// forward declarations
struct PORT_t;

#define PORTA ((PORT_t*) 0x005000)
#define PORTB ((PORT_t*) 0x005005)
#define PORTC ((PORT_t*) 0x00500A)
#define PORTD ((PORT_t*) 0x00500F)
#define PORTE ((PORT_t*) 0x005014)
#define PORTF ((PORT_t*) 0x005019)
*/


/*
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


PORT_t* pin_to_port(u8 pin);
u8 pin_to_pos(u8 pin);
void gpio_mode_in(u8 pin);
void gpio_mode_out(u8 pin);
void gpio_mode_pullup(u8 pin);
void gpio_toggle(u8 pin);
u8 gpio_read(u8 pin);
void gpio_write(u8 pin, u8 val);
*/


