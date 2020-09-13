#pragma once

#include <stdint.h>


#define __IO volatile

// section 3.3 Memory map page 50

struct GPIO_t;
#define GPIOC ((GPIO_t*)   0x40011000)
#define GPIOB ((GPIO_t*)   0x40010C00)
#define GPIOA ((GPIO_t*)   0x40010800)
struct TIMx_t;
#define TIM4	((TIMx_t*) 0x40000800)

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

