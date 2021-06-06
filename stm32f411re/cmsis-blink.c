#include "stm32f4xx.h"

void delay (uint32_t time) 
{    
    static uint32_t i;
    for (i=0; i<time; i++) {}    
}

int main (void) {   

    RCC->AHB1ENR     |= RCC_AHB1ENR_GPIOCEN; //RCC ON

    GPIOC->MODER    |= GPIO_MODER_MODER13_0; //mode out
    GPIOC->OTYPER   = 0;
    GPIOC->OSPEEDR  = 0;
    
    while (1) 
    {
        GPIOC->ODR ^=   GPIO_ODR_OD13;
        delay(1000000);
    }
}