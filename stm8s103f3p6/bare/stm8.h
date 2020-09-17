#pragma once

#include <stdint.h>

typedef uint16_t u16;
typedef unsigned char uchar;
typedef unsigned long  ulong;

// forward declarations
struct PORT_t;

#define PORTA ((PORT_t*) 0x005000)
#define PORTB ((PORT_t*) 0x005005)
#define PORTC ((PORT_t*) 0x00500A)
#define PORTD ((PORT_t*) 0x00500F)
#define PORTE ((PORT_t*) 0x005014)
#define PORTF ((PORT_t*) 0x005019)
#define CLK_DIVR *(uchar*)0x0050C6
#define TIM2_CR1 *(uchar*)0x005300 // control register
#define TIM2_EGR *(uchar*)0x005306 // Event Generation Register
#define TIM2_CNTRH *(uchar*)0x00530C  // counter high
#define TIM2_CNTRL *(uchar*)0x00530D  // counter low
#define TIM2_PSCR *(uchar*)0x00530E // prescaler

#define TIM2_CR1_CEN (1<<0) // counter enable
#define TIM2_EGR_UG (1<<0) // update generation

#define TIM2_ARRH *(uint8_t*)0x530F // Auto reload register high
#define TIM2_ARRL *(uint8_t*)0x5310 // Auto reload register high
#define TIM2_CCR1H *(uint8_t*)0x5311  // Capture compare register 1 high
#define TIM2_CCR1L *(uint8_t*)0x5312  // Capture compare register 1 low
#define TIM2_CCER1 *(uint8_t*)0x530A // apture compare enable register 1
#define TIM2_CCER1_CC1P (1<<1)
#define TIM2_CCER1_CC1E (1<<0)
#define TIM2_CCMR1 *(uint8_t*)0x5307  // Capture/compare mod register 1


typedef struct {
	uchar ODR;
	uchar IDR;
	uchar DDR;
	uchar CR1;
	uchar CR2;
} PORT_t;

#define  enable_interrupts() __asm__("rim");
#define disable_interrupts() __asm__("sim");
#define set_bit(register_8, bit) (register_8 |= (1 << bit))
#define clear_bit(register_8, bit) (register_8 &= ~(1 << bit))
#define toggle_bit(register_8, bit) (register_8 ^= (1 << bit))

