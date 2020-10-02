#pragma once

#include <stdint.h>


typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

#define __IO volatile

#define REG(x) *(volatile uint32_t *)(x)

#define RCC_BASE      	0x40021000


#define RTC_BASE 0x40002800
#define RTC_CNTH *(volatile uint32_t *)(RTC_BASE   + 0x18)
#define RTC_CNTL *(volatile uint32_t *)(RTC_BASE   + 0x1C)



// section 3.3 Memory map page 50

// forward declarations
struct GPIO_t;
struct SPI_t;
struct TIMx_t;
struct USART_t;

#define GPIO_BASE 0x40010800

#define USART1 ((USART_t*) 0x40013400)

#define SPI1 ((SPI_t*) 0x40013000)
#define SPI3 ((SPI_t*) 0x40003C00)
#define SPI2 ((SPI_t*) 0x40003800)


#define GPIOC ((GPIO_t*) (GPIO_BASE + 0x800))
#define GPIOB ((GPIO_t*) (GPIO_BASE + 0x400))
#define GPIOA ((GPIO_t*) (GPIO_BASE + 0x000))
#define USART5 ((USART_t*) 0x40005000)
#define USART4 ((USART_t*) 0x40004C00)
#define USART3 ((USART_t*) 0x40004800)
#define USART2 ((USART_t*) 0x40004400)
#define TIM4	((TIMx_t*) 0x40000800)
#define TIM2	((TIMx_t*) 0x40000000)

// section 7.3.11 RCC register map page 121
#define RCC_CR   *(volatile uint32_t *)(RCC_BASE + 0x00) 
#define RCC_CR_HSION (1<<0)
#define RCC_CR_HSIRDY (1<<1)
#define RCC_CFGR   *(volatile uint32_t *)(RCC_BASE + 0x04) 
#define RCC_CFGR_SW (1<<0)
#define RCC_APB1ENR   *(volatile uint32_t *)(RCC_BASE   + 0x1C) // page 148
#define RCC_APB1ENR_TIM4EN (1<<2)
#define RCC_APB1ENR_USART2EN	(1<<17)
#define RCC_APB2ENR   *(volatile uint32_t *)(RCC_BASE   + 0x18)
#define RCC_APB2ENR_IOPAEN	(1<<2)
#define GPIOA_CRL     *(volatile uint32_t *)(GPIOA_BASE + 0x00)

#define GPIO_CRL_CNF2_Pos 10 // page 171
#define GPIO_CRL_CNF3_Pos 14
#define GPIO_CRL_MODE2_Pos 8
#define GPIO_CRL_MODE3_Pos 12
#define GPIO_CRL_CNF2 (0x3<<GPIO_CRL_CNF2_Pos)
#define GPIO_CRL_CNF3 (0x3<<GPIO_CRL_CNF3_Pos)
#define GPIO_CRL_MODE2 (0x3<<GPIO_CRL_MODE2_Pos)
#define GPIO_CRL_MODE3 (0x3<<GPIO_CRL_MODE3_Pos)

#define USART_BRR_DIV_Fraction_Pos 0 // page 820
#define USART_BRR_DIV_Mantissa_Pos 4
#define USART_CR1_RE (1<<2) // page 821
#define USART_CR1_TE (1<<3)
#define USART_CR1_UE (1<<13)
#define USART_SR_RXNE (1 << 5) // page 818
#define USART_SR_TXE (1 << 7)
// page 194
typedef struct
{
	__IO uint32_t CRL; // 0x00
	__IO uint32_t CRH; // 0x04
	__IO uint32_t IDR; // 0x08
	__IO uint32_t ODR; // 0x0C
	__IO uint32_t BSRR; // 0x10
	__IO uint32_t BRR; // 0x14
	__IO uint32_t LCKR; //0x18
} GPIO_t;

// section 25.5 page 742
typedef struct {
	__IO u32 CR1;     // 0x00
	__IO u32 CR2;     // 0x04
	__IO u32 SR;      // 0x08
	__IO u32 DR;      // 0x0C
	__IO u32 CRCPR;   // 0x10
	__IO u32 RXCRCR;  // 0x14
	__IO u32 TXCRCR;  // 0x18
	__IO u32 I2SCFGR; // 0x1C
	__IO u32 I2SPR;   // 0x20
} SPI_t;


// section 15.4.18 TIMx register map page 423
typedef struct {
	__IO uint32_t CR1; // 0x00
	__IO uint32_t CR2; // 0x04
	__IO uint32_t SMCR; // 0x08
	__IO uint32_t DIER; // 0x0C
	__IO uint32_t SR; // 0x10
	__IO uint32_t EGR; // 0x14
	__IO uint32_t CCMR1; // 0x18
	__IO uint32_t CCMR2; // 0x1C
	__IO uint32_t CCER; // 0x20
	__IO uint32_t CNT; // 0x24
	__IO uint32_t PSC; // 0x28
	__IO uint32_t ARR; // 0x2C
	// ... more ... incomplete
} TIMx_t;

#define TIM_CR1_CEN (1<<0)

// USART register map: page 827
typedef struct {
	__IO uint32_t SR; // 0x00
	__IO uint32_t DR; // 0x04
	__IO uint32_t BRR; // 0x08
	__IO uint32_t CR1; // 0x0C
	__IO uint32_t CR2; // 0x10
	__IO uint32_t CR3; // 0x14
	__IO uint32_t GTPT; // 0x18
} USART_t;

void init_serial();
char* itoa(int num, char* str, int base);
void put32(u32 addr, u32 val);
u32 get32(u32 addr);
#define disable_irq() asm("CPSID I")
#define enable_irq() asm("CPSIE I")
