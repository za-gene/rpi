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

#define UART1_SR   *(uchar*)(0x5230)
#define UART1_DR   *(uchar*)(0x5231)
#define UART1_BRR1 *(uchar*)(0x5232)
#define UART1_BRR2 *(uchar*)(0x5233)
#define UART1_CR1  *(uchar*)(0x5234)
#define UART1_CR2  *(uchar*)(0x5235)
#define UART1_CR3  *(uchar*)(0x5236)
#define UART1_CR4  *(uchar*)(0x5237)
#define UART1_CR5  *(uchar*)(0x5238)
#define UART1_GTR  *(uchar*)(0x5239)
#define UART1_PSCR *(uchar*)(0x523A)

#define TIM2_CR1 *(uchar*)0x005300 // control register
#define TIM2_IER *(uchar*)0x005303 // Interrupt Enable Register
#define TIM2_SR1 *(uchar*)0x005304 // Status Register 1
#define TIM2_EGR *(uchar*)0x005306 // Event Generation Register
#define TIM2_CCMR1 *(uint8_t*)0x5307  // Capture/compare mod register 1
#define TIM2_CNTRH *(uchar*)0x00530C  // counter high
#define TIM2_CNTRL *(uchar*)0x00530D  // counter low
#define TIM2_PSCR *(uchar*)0x00530E // prescaler
#define TIM2_ARRH *(uint8_t*)0x530F // Auto reload register high
#define TIM2_ARRL *(uint8_t*)0x5310 // Auto reload register high
#define TIM2_CCR1H *(uint8_t*)0x5311  // Capture compare register 1 high
#define TIM2_CCR1L *(uint8_t*)0x5312  // Capture compare register 1 low
#define TIM2_CCER1 *(uint8_t*)0x530A // apture compare enable register 1




typedef struct {
	uchar ODR;
	uchar IDR;
	uchar DDR;
	uchar CR1;
	uchar CR2;
} PORT_t;


#define TIM2_CR1_CEN (1<<0) // counter enable
#define TIM2_EGR_UG (1<<0) // update generation

#define TIM2_CCER1_CC1P (1<<1)
#define TIM2_CCER1_CC1E (1<<0)
#define TIM2_SR1_UIF (1<<0) // Update Interrupt Flag
#define TIM2_IER_UIE (1<<0) 


#define UART1_CR1_M (1<<4)
#define UART1_CR1_PCEN (1<<2)

#define UART1_CR2_TEN           (1<<3)
#define UART1_CR2_REN           (1<<2)

#define UART1_CR3_STOP 4
#define UART1_CR3_CPOL (1<<2)
#define UART1_CR3_CPHA (1<<1)
#define UART1_CR3_LBCL (1<<0)
#define UART1_CR3_CLKEN (1<<3)

#define UART1_SR_TXE            (1<<7)
#define UART1_SR_TC             (1<<6)
#define UART1_SR_RXNE           (1<<5)



// Interrupts
#define TLI_ISR                 0
#define AWU_ISR                 1
#define CLK_ISR                 2
#define EXTI0_ISR               3
#define EXTI1_ISR               4
#define EXTI2_ISR               5
#define EXTI3_ISR               6
#define EXTI4_ISR               7
#define SPI_ISR                 10
#define TIM1_OVF_ISR            11
#define TIM1_CC_ISR             12
#define TIM2_OVF_ISR            13
#define TIM2_CC_ISR             14
#define UART1_TXC_ISR           17
#define UART1_RXC_ISR           18
#define I2C_ISR                 19
#define ADC1_ISR                22
#define TIM4_ISR                23
#define FLASH_ISR               24


#define  enable_interrupts() __asm__("rim");
#define disable_interrupts() __asm__("sim");
#define set_bit(register_8, bit) (register_8 |= (1 << bit))
#define clear_bit(register_8, bit) (register_8 &= ~(1 << bit))
#define toggle_bit(register_8, bit) (register_8 ^= (1 << bit))

