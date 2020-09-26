#pragma once

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef unsigned char uchar;
typedef unsigned long  ulong;


// MEMORY MAP

// forward declarations
struct PORT_t;

#define PORTA ((PORT_t*) 0x005000)
#define PORTB ((PORT_t*) 0x005005)
#define PORTC ((PORT_t*) 0x00500A)
#define PORTD ((PORT_t*) 0x00500F)
#define PORTE ((PORT_t*) 0x005014)
#define PORTF ((PORT_t*) 0x005019)

#define CLK_ICKR *(uchar*)0x50c0
#define CLK_ECKR *(uchar*)0x50c1
// 0x50C2 reserved
#define CLK_SWR *(uchar*)0x50c4
#define CLK_SWCR *(uchar*)0x50c5
#define CLK_CKDIVR *(uchar*)0x0050C6 // (changed from CLK_DIVR 2020-09-19)
#define CLK_PCKENR1 *(uchar*)0x50c7
#define CLK_CCOR *(uchar*)0x50c9
#define CLK_PCKENR2 *(uchar*)0x50ca
#define CLK_HSITRIMR *(uchar*)0x50cc
#define CLK_SWIMCCR *(uchar*)0x50cd

#define SPI_CR1 *(uchar*)0x005200
#define SPI_CR2 *(uchar*)0x005201
#define SPI_ICR *(uchar*)0x005202
#define SPI_SR *(volatile uchar*)0x005203
#define SPI_DR *(uchar*)0x005204
#define SPI_CRCPR *(uchar*)0x005205 // CRC polynomial register
#define SPI_RXCRCR *(uchar*)0x005206 // Rx CRC register
#define SPI_TXCRCR 0*(uchar*)x005207 // Tx CRC register

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

#define TIM4_CR1 *(uint8_t*)0x5340
#define TIM4_IER *(uint8_t*)0x5343
#define TIM4_SR *(uint8_t*)0x5344
#define TIM4_PSCR *(uint8_t*)0x5347
#define TIM4_ARR *(uint8_t*)0x5348



typedef struct {
	uchar ODR;
	uchar IDR;
	uchar DDR;
	uchar CR1;
	uchar CR2;
} PORT_t;


// END MEMORY MAP


#define CLK_ICKR_HSIEN (1<<0)
#define CLK_ICKR_HSIRDY (1<<1)

#define CLK_SWCR_SWBSY (1<<0) 
#define CLK_SWCR_SWEN (1<<1) 

#define TIM2_CR1_CEN (1<<0) // counter enable
#define TIM2_EGR_UG (1<<0) // update generation

#define TIM2_CCER1_CC1P (1<<1)
#define TIM2_CCER1_CC1E (1<<0)
#define TIM2_SR1_UIF (1<<0) // Update Interrupt Flag
#define TIM2_IER_UIE (1<<0) 

// section 19.6.10 page 260 TIM4/TIM6 register map and reset values
#define TIM4_IER_UIE (1<<0)
#define TIM4_CR1_CEN (1<<0)
#define TIM4_SR_UIF (1<<0)

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


#define SPI_CR1_MSTR (1<<2)
#define SPI_CR1_SPE (1<<6)

// SPI Interrupt Control Register section 20.4.3 page 284
#define SPI_ICR_TXIE (1<<7)
#define SPI_ICR_RXIE (1<<6)

#define SPI_CR2_SSM (1<<1)

// SPI status register section 20.4.4 page 285
#define SPI_SR_RXNE (1<<0) 
#define SPI_SR_TXE (1<<1) 
#define SPI_SR_WKUP (1<<3)
#define SPI_SR_CRCERR (1<<4)
#define SPI_SR_MODF (1<<5) 
#define SPI_SR_OVR (1<<6) 
#define SPI_SR_BSY (1<<7) 



// INTERRUPTS
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


#define nop() __asm__("nop")
#define enable_interrupts() __asm__("rim");
#define disable_interrupts() __asm__("sim");
#define set_bit(register_8, bit) (register_8 |= (1 << bit))
#define clear_bit(register_8, bit) (register_8 &= ~(1 << bit))
#define toggle_bit(register_8, bit) (register_8 ^= (1 << bit))


#include <gpio.h>
