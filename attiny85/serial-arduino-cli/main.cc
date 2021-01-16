#define F_CPU 1000000UL


//#include <avr/io.h>
#include <util/delay.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include <timer1.h>

ISR(TIM1_COMPA_vect)
{
	PORTB ^= (1<<PB1); // toggle pin
}



//SendOnlySoftwareSerial ser(3);




int main()
{
	DDRB = 1<<PB1; // set PB1 for output
	PORTB = 0x00;  // set all pins low
	init_timer1(F_CPU, 9600); 

	for(;;) {
		_delay_ms(1);
		//PORTB ^= (1<<PB1);
	}
}
