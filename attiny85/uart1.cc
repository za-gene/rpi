#include <uart1.h>

//#include <util/delay.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include <timer1.h>


//typedef uint8_t u8;

//volatile u8 tx_rdy = 1;
//volatile u8 tx_bit = 1;



volatile uint8_t UART1;

/* bit layout of UART1
 * 76513210
 * PPP...RB
 * P: pin number of TX (.e.g PB1)
 * R: Ready bit
 * B: Bit to send
 */

#define UART1_BIT (1<<0) // bit to send
#define UART1_RDY (1<<1) // ready?
#define UART1_TX  (1<<5) // TX gpio pin

ISR(TIMER1_COMPA_vect)
{
	uint8_t tx = 1 << (UART1 >> UART1_TX); // get TX gpio pin
	if(UART1 & UART1_BIT)
		PORTB |= tx;
	else
		PORTB &= ~tx;
	//tx_rdy = 1;
	UART1 |= UART1_RDY;
}




void send_bit(u8 b)
{
	while(!(UART1 & UART1_RDY));

	//tx_bit = b;
	if(b)
		UART1 |= UART1_BIT;
	else
		UART1 &= ~UART1_BIT; 
	//tx_rdy = 0;
	UART1 &= ~UART1_RDY;
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



void init_uart1(unsigned long f_cpu. uint8_t tx_pin)
{
	PORTB |= (1<<tx_pin); // set TX pin high
	DDRB |= (1<<tx_pin);  // set TX pin as output pin
	//UART1 = 0;
	UART1 = tx_pin << UART1_TX;
	UART1 |= UART1_RDY; // set serial as ready to transmit
	init_timer1(f_cpu, 9600); 
}
