#include <gpio.h>

#define TIM2_CR1 *(uchar*)0x005300 // control register
#define TIM2_EGR *(uchar*)0x005306 // Event Generation Register
#define TIM2_CNTRH *(uchar*)0x00530C  // counter high
#define TIM2_CNTRL *(uchar*)0x00530D  // counter low
#define TIM2_PSCR *(uchar*)0x00530E // prescaler

#define TIM2_CR1_CEN (1<<0) // counter enable
#define TIM2_EGR_UG (1<<0) // update generation

//#define LED (1<<5) // inbuilt led

void delay(u16 ms)
{
	//initialise timer (it doesn't really matter if we do it multiple times
	// Default clock is HSI/8 = 2MHz
	TIM2_PSCR = 0b00000111; //  Prescaler = 128
	// Generate an update event so prescaler value will be taken into account.
	TIM2_EGR |= TIM2_EGR_UG;
	TIM2_CR1 |= TIM2_CR1_CEN; // Enable TIM2

	// Reset counter back to 0
	TIM2_CNTRH = 0;
	TIM2_CNTRL = 0;
	while(1) {
		u16 tim = ((u16)TIM2_CNTRH << 8)+ (u16)TIM2_CNTRL;
		// each tick is 64us long. 1/2MHz = 0.5us. 0.5us*128 = 64us
		// Therefore 1ms takes 1000/64us ticks
		if(tim >= ms * (1000/64)) break;
	}
}


int main()
{
	pinMode(LED_BUILTIN, OUTPUT);
	//PORTB->DDR |= LED; // PB5 is now output
	//PORTB->CR1 |= LED; // PB5 is now pushpull

	while (1) {
		digitalToggle(LED_BUILTIN);
		//PORTB->ODR ^= LED; // toggle
		delay(1000);
	}
}
