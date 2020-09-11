#include "../blue.h"

// http://embedded-lab.com/blog/stm32-gpio-ports-insights/

/* A more refactored style for the registers can be found at
 * https://github.com/WRansohoff/STM32x0_timer_example/blob/master/device_headers/stm32f031x6.h
 */

// register address
#define RCC_BASE      0x40021000


#define RCC_APB2ENR   *(volatile uint32_t *)(RCC_BASE   + 0x18)


// bit fields
#define RCC_IOPBEN   (1<<3)
#define RCC_IOPCEN   (1<<4)
#define GPIO1       (1UL<<1)
#define GPIO13      (1UL<<13)

//static int j = 666; // this seems to cause problems

void main(void)
{
	RCC_APB2ENR |= RCC_IOPCEN; // enable port C
	GPIOC->CRH   &= 0xFF0FFFFF;
	GPIOC->CRH   |= 0x00200000;

	// Set PB13 into a high output state
	RCC_APB2ENR |= RCC_IOPBEN; // enable port B
	// set PB13 for output
	GPIOB->CRH   &= 0xFF0FFFFF;
	GPIOB->CRH   |= 0x00200000;
	// set pin PB13 high
	GPIOB->ODR |= GPIO13;

	// set PB1 for output and set it high
	GPIOB->CRL  &= 0xFFFFFF0F;
	GPIOB->CRL  |= 0x00000020;
	GPIOB->ODR  |= GPIO1;


	int i = 666;
	while(1)
	{
		//GPIOC_ODR |=  GPIO13;
		GPIOC->ODR |= GPIO13;
		for (i = 0; i < 500000; i++); // arbitrary delay
		GPIOC->ODR &= ~GPIO13;
		for (i = 0; i < 500000; i++); // arbitrary delay
	}
}


