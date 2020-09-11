#pragma once

#include <stdint.h>


#define __IO volatile

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

// reference page 51
#define GPIOA ((GPIO_t*) 0x40010800)
#define GPIOB ((GPIO_t*) 0x40010C00)
#define GPIOC ((GPIO_t*) 0x40011000)
