#define F_CPU 8000000UL
//#include "minitone85.h"

#include <avr/io.h>
#include <avr/iotnx5.h> // for CLKPR
#include <avr/interrupt.h>
//#include <util/delay.h>
#include <stdlib.h>

#include "gpio.h"
#include "timer1.h"

#define NOISE_PIN 	PB2
#define TONE_PIN	PB1

/* generating noise seems to interfere with PWM, so just use a timer interrupt.
 * I think digitalWrite is non-atomic, which causes problems for output.
 */

ISR(TIMER1_COMPA_vect)
{
	volatile static uint8_t on = 0;
	on = 1 - on;
	digitalWrite(TONE_PIN, on);
}


int main()
{
	// scale chip to 8Mhz
	CLKPR = 1<<CLKPCE;
	CLKPR = 0; 

	pinMode(TONE_PIN, OUTPUT);
	init_timer1(F_CPU, 440*2);
	//miniTone(440); 

	pinMode(NOISE_PIN, OUTPUT);
	for(;;) {
		uint8_t on = random() & 1;
		cli();
		digitalWrite(NOISE_PIN, on);
		sei();
	}
}
