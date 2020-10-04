#include <gpio.h>
#include <spi.h>



#define rs_pin PB0
#define cs_pin PA4

#define digitalWrite gpio_write


/* this seems necessary for the LCD to respond to commands
 * properly. A delay of 5000 doesn't seem adequate
 */

void dog_delay()
{
	for(int i=0; i< 6000; i++) nop(); // simple delay to allow for catchup
}

void sendByte(int rs_val, int val) {
	digitalWrite(rs_pin, rs_val);
	digitalWrite(cs_pin, LOW);
	spi_transfer(val);
	digitalWrite(cs_pin, HIGH);  
	dog_delay();
}


void main()
{
	gpio_mode(cs_pin, OUTPUT);
	gpio_write(cs_pin, HIGH);
	gpio_mode(rs_pin, OUTPUT);


	dog_delay();
	init_spi();
	dog_delay();

	u8 contrast = 0x70  | 0b1000; // from 0x7C
	u8 display = 0b1111; // ori 0x0F
	uint8_t cmds[] = {0x39, 0x1D, 0x50, 0x6C, contrast , 0x38, display, 0x01, 0x06};
	for(int i=0; i<sizeof(cmds); ++i) sendByte(LOW, cmds[i]);

	// now send some interesting output
	uint8_t msg[] = {'S', 'T', 'M', '3', '2'};
	for(int i=0; i<sizeof(msg); ++i) sendByte(HIGH, msg[i]);

	while(1);
}




