#include <stdint.h>

// http://embedded-lab.com/blog/stm32-gpio-ports-insights/

// register address
#define RCC_BASE      0x40021000
// reference page 51
#define GPIOB_BASE    0x40010C00
#define GPIOC_BASE    0x40011000

#define RCC_APB2ENR   *(volatile uint32_t *)(RCC_BASE   + 0x18)
#define GPIOB_CRL     *(volatile uint32_t *)(GPIOB_BASE + 0x00)
#define GPIOB_CRH     *(volatile uint32_t *)(GPIOB_BASE + 0x04)
#define GPIOB_ODR     *(volatile uint32_t *)(GPIOB_BASE + 0x0C)
#define GPIOC_CRH     *(volatile uint32_t *)(GPIOC_BASE + 0x04)
#define GPIOC_ODR     *(volatile uint32_t *)(GPIOC_BASE + 0x0C)

// bit fields
#define RCC_IOPBEN   (1<<3)
#define RCC_IOPCEN   (1<<4)
#define GPIO1       (1UL<<1)
#define GPIO13      (1UL<<13)

//static int j = 666; // this seems to cause problems

void main(void)
{
    RCC_APB2ENR |= RCC_IOPCEN; // enable port C
    GPIOC_CRH   &= 0xFF0FFFFF;
    GPIOC_CRH   |= 0x00200000;

    // Set PB13 into a high output state
    RCC_APB2ENR |= RCC_IOPBEN; // enable port B
    // set PB13 for output
    GPIOB_CRH   &= 0xFF0FFFFF;
    GPIOB_CRH   |= 0x00200000;
    // set pin PB13 high
    GPIOB_ODR |= GPIO13;

    // set PB1 for output and set it high
    GPIOB_CRL  &= 0xFFFFFF0F;
    GPIOB_CRL  |= 0x00000020;
    GPIOB_ODR  |= GPIO1;


    int i = 666;
    while(1)
    {
        GPIOC_ODR |=  GPIO13;
	//int i = 666; // so we can play around with debugging
        for (i = 0; i < 500000; i++); // arbitrary delay
        GPIOC_ODR &= ~GPIO13;
        for (i = 0; i < 500000; i++); // arbitrary delay
    }
}


