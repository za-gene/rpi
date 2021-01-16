#define F_CPU 1000000UL


//#include <avr/io.h>
#include <util/delay.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include <timer1.h>

#define TX (1<<PB1)

typedef uint8_t u8;

volatile u8 tx_rdy = 1;
volatile u8 tx_bit = 1;
//u8 tx_send = 0;

ISR(TIMER1_COMPA_vect)
{
	//if(!tx_send) return;

	//PORTB = ~ PORTB;
	//return;

	if(tx_bit)
		PORTB |= TX;
	else
		PORTB &= ~TX;
	tx_rdy = 1;
}



//SendOnlySoftwareSerial ser(3);

void send_bit(u8 b)
{
	//tx_done = 0;
	//tx_send = 1;
	while(!tx_rdy);
	tx_bit = b;
	tx_rdy = 0;
	//tx_send = 1;

}

void send(u8 c)
{
	send_bit(0); // start bit
	for(u8 i = 0; i < 8; i++) {
		send_bit(c & 1); // send LSB
		c >>= 1; // shift right
	}
	send_bit(1); // stop bit
	send_bit(1); // stop bit

}



int main()
{
	PORTB = TX; // set TX output high
	DDRB = TX; // set TX for output
	//PORTB = 0x00;  // set all pins low
	init_timer1(F_CPU, 9600); 
	//init_timer1(F_CPU, 115200); 

	for(;;) {
		for(int c = 'A'; c < 'E'; c++)
			send(c);
		send('\r');
		send('\n');
		_delay_ms(1000);
		//PORTB ^= (1<<PB1);
	}
}
