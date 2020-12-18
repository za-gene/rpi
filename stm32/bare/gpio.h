#pragma once
#include "blue.h"

// http://embedded-lab.com/blog/stm32-gpio-ports-insights/

/* A more refactored style for the registers can be found at
 * https://github.com/WRansohoff/STM32x0_timer_example/blob/master/device_headers/stm32f031x6.h
 */

#ifdef __cplusplus
extern "C" {
#endif


#define LOW 0
#define HIGH 1

// pins are defined as hex 0xPQ where P is port, and Q is pin number
// Currently incomplete

#define PA4  0x04
#define PA5  0x05
#define PA6  0x06
#define PA7  0x07
#define PA9  0x09
#define PA10 0x0A
#define PA13 0x0D
#define PB0  0x10
#define PB1  0x11
#define PB6  0x16
#define PB7  0x17
#define PB10 0x1A
#define PB11 0x1B
#define PB12 0x1C
#define PB13 0x1D
#define PC13 0x2D
#define PC15 0x2F

#define BUILTIN_LED PC13

// section 9.2.1 and 9.2.2 CNF/MODE combos
#define OSPEED 0b10 // 2MHz
#define INPUT  		0b0100 // CNF=1, MODE=0, floating input
#define INPUT_PULLUP 	0b1000 // CNF=2, MODE =0. input with pull-up/down
#define OUTPUT 		(0b0000 | OSPEED) // CNF=0, MODE=1, output push-pull ("regular" out)
#define OUTPUT_ALT	(0b1000 | OSPEED) // alt out push-pull

u32 pin_to_gpio(u32 pin);
void gpio_write(u32 pin, u32 val);
void gpio_mode(u32 pin, u8 mode);
void gpio_mode_out(u32 pin);
void gpio_toggle(u32 pin);
u8 gpio_read(u32 pin);

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

#define GPIOC ((GPIO_t*) (GPIO_BASE + 0x800))
#define GPIOB ((GPIO_t*) (GPIO_BASE + 0x400))
#define GPIOA ((GPIO_t*) (GPIO_BASE + 0x000))


#define GPIOA_CRL     *(volatile uint32_t *)(GPIOA_BASE + 0x00)

#define GPIO_CRL_CNF2_Pos 10 // page 171
#define GPIO_CRL_CNF3_Pos 14
#define GPIO_CRL_MODE2_Pos 8
#define GPIO_CRL_MODE3_Pos 12
#define GPIO_CRL_CNF2 (0x3<<GPIO_CRL_CNF2_Pos)
#define GPIO_CRL_CNF3 (0x3<<GPIO_CRL_CNF3_Pos)
#define GPIO_CRL_MODE2 (0x3<<GPIO_CRL_MODE2_Pos)
#define GPIO_CRL_MODE3 (0x3<<GPIO_CRL_MODE3_Pos)


#ifdef __cplusplus
}
#endif
