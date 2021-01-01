#define F_CPU 1000000UL


//#include <avr/io.h>
#include <util/delay.h>

#include <avr/io.h>
#include <avr/interrupt.h>

void set_timer1(unsigned long freq)
{
	TCCR1 = 0; // stop the counter
	TCNT1 = 0;
	OCR1C = 0;
	if(freq==0) return;

	freq *= 2;
	int prescale = 1;
	while(freq * 256 * (1<<(prescale-1)) < F_CPU)
		prescale++;
	TCCR1 = (1<<CTC1) // clear timer on compare match
	//	| (1<<COM1A0) 
		| prescale;
	int match = F_CPU/(1<<(prescale-1))/freq -1;
	OCR1A = match; // generate interrupt on match to drive ISR
	//OCR1C = match; // clear counter on match to define period
}





ISR(TIM1_COMPA_vect)
{
	PORTB ^= (1<<PB1); // toggle pin
}



//SendOnlySoftwareSerial ser(3);




int main()
{
	DDRB = 1<<PB1; // set PB1 for output
	PORTB = 0x00;  // set all pins low
	set_timer1(9600); 
	//TIMSK |= (1<<TOIE1); // enable interrupt for timer1
	TIMSK |= (1<<OCIE1A); // enable interrupt for timer1
	sei();

	for(;;) {
		_delay_ms(1);
		//PORTB ^= (1<<PB1);
	}
}
