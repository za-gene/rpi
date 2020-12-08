// not yet working
#include <bcm2835.h>
//#include <mini_uart.h>
#include <stdio.h>
#include <timers.h>


#define SID 0x70 // Slave ID

typedef uint8_t u8;

static uint8_t  pattern[] = { 
	0b10000001,
	0b01000010,
	0b00100100,
	0b00010000,
	0b00001000,
	0b00100100,
	0b01000010,
	0b10000001
};

void send_cmd(u8 cmd) {
	//Wire.beginTransmission(SID);
	//Wire.write(cmd);
	//Wire.endTransmission();
	u8 b = cmd;
	bcm2835_i2c_write(&b, 1);
}


void write_row(uint8_t y, uint8_t xs) 
{
	// the row of xs beed to be quirkily transformed
	uint8_t bits = 0;
	for (int x = 0; x < 7; ++x) {
		bits <<= 1;
		bits |= (xs & 1);
		xs  >>= 1;
	}
	bits |= (xs << 7);

	char buf[2];
	buf[0] = 2*y;
	buf[1] = bits;
	bcm2835_i2c_write(buf, 2);
	//Wire.beginTransmission(SID); // address of LED matrix
	//Wire.write(2 * y);
	//Wire.write(bits);
	//Wire.endTransmission();

}


void kernel_main(void)
{
	//uart_init_as_stdio(115200);
	puts("bcm spi master example");
	bcm2835_init();
	bcm2835_i2c_begin();
	bcm2835_i2c_set_baudrate(100000);
	bcm2835_i2c_setSlaveAddress(SID);

	send_cmd(0x20 | 1); // turn on oscillator
	send_cmd(0x81); // display on
	send_cmd(0xE0 | 0); // brightness to dimmest (but you should probably set it)

	while (1) {
		for (int i = 0; i < 8; ++i) {
			u8 row = pattern[i];
			write_row(i, row);
			row = ((row & 0x01) ? 0x80 : 0x00) | (row >> 1); // rotate the row for fun
			pattern[i] = row;
		}
		delay(100);
	}
}
