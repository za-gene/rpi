// blink the in-built LED

#include <gpio.h>


//typedef unsigned char uchar;
//typedef unsigned long  ulong;

#define CLK_DIVR *(uchar*)0x50C6

/*
struct PORT_t;

#define PORTA ((PORT_t*) 0x005000)
#define PORTB ((PORT_t*) 0x005005)
#define PORTC ((PORT_t*) 0x00500A)
#define PORTD ((PORT_t*) 0x00500F)
#define PORTE ((PORT_t*) 0x005014)
#define PORTF ((PORT_t*) 0x005019)

typedef struct {
	uchar ODR;
	uchar IDR;
	uchar DDR;
	uchar CR1;
	uchar CR2;
} PORT_t;

*/

ulong dl; // delay loop

// inbuilt LED is on pin B5
//PORT_t* PORT = PORTB;
//#define PIN (1<<5) 

int main()
{
	CLK_DIVR = 0x00; // set clock to 16MHz
	pinMode(LED_BUILTIN, OUTPUT);
	//PORT->ODR = 0; // turn all pins of port low

	// set pin to "push-pull" output
	//PORT->DDR = PIN; // pin is output
	//PORT->CR1 = PIN; // pin is now push-pull output

	//PORT->ODR |= PIN; // turn on pin
	while(1) {
		digitalToggle(LED_BUILTIN);
		//PORT->ODR ^= PIN; // toggle
		for(dl = 0; dl<200000UL; dl++);
	}
}
