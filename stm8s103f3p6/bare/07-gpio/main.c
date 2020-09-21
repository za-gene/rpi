#include "../stm8.h"


#define LED (1<<5)

void main()
{
	init_millis();

	PORTB->DDR |= LED;  // LED is output
	PORTB->CR1 |= LED;  // LED is pushpull (normal)

	u32 ms = millis();
	while (1)
	{
		if(millis() - ms < 500) continue;
		ms = millis();
		PORTB->ODR ^= LED; // Toggle LED
	}
}

