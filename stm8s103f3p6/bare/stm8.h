#pragma once

#include <stdint.h>

typedef uint16_t u16;
typedef unsigned char uchar;
typedef unsigned long  ulong;

#define CLK_DIVR *(uchar*)0x50C6

struct PORT_t;

#define PORTA ((PORT_t*) 0x005000)
#define PORTB ((PORT_t*) 0x005005)
#define PORTC ((PORT_t*) 0x00500A)
#define PORTD ((PORT_t*) 0x00500F)
#define PORTE ((PORT_t*) 0x005014)
#define PORTF ((PORT_t*) 0x005019)

typedef struct {
	uchar ODR;
	uchar IDR;
	uchar DDR;
	uchar CR1;
	uchar CR2;
} PORT_t;

