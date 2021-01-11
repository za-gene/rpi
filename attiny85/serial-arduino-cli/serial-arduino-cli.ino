#define F_CPU 1000000UL

#include <avr/io.h> // for TCCR1, OR1C

//#include <MsTimer2.h>

typedef uint8_t u8;

void enable_timer1_irq(unsigned long freq)
{
	//uint8_t sreg = SREG; // remember interrupts
	cli(); // stop interrupts

	TCCR1 = 0; // stop the counter
	TCNT1 = 0;
	OCR1A = 0;
	OCR1B = 0;
	//OCR1C = 0;
	if(freq==0) return;

	int prescale = 1;
	while(freq * 256 * (1<<(prescale-1)) < F_CPU)
		prescale++;
	TCCR1 = (1<<CTC1) | prescale;
	//TCCR1 = prescale;
//TCCR1 = 1;
	int divisor = F_CPU/(1<<(prescale-1))/freq;
	OCR1A = divisor; // set the Output Compare Register
OCR1A = 200;
	sei();
	TIMSK =  _BV(OCIE1A);
	//TIMSK = _BV(TOIE1);

	//SREG = sreg; // restore interrupts
}




#define TX PB3

ISR (TIM1_COMPA_vect) 
{
	//TCNT1 = 0;
	digitalWrite(TX, 1- digitalRead(TX));
}


void flash()
{
	digitalWrite(TX, 1- digitalRead(TX));
}
	

void setup() 
{ 
	pinMode(TX, OUTPUT);
	enable_timer1_irq(9600);
 //MsTimer2::set(104, flash); // 500ms period
  //MsTimer2::start();
           
}

void loop() 
{
}

