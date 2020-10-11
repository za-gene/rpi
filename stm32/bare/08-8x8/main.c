#include <dma.h>
#include <gpio.h>
#include <i2c.h>
#include <usart.h>

#define SID 0x70 // SID


static uint8_t  pattern[] = { 
	//0b11111111, // for testing purposes
	0b10000001,
	0b01000010,
	0b00100100,
	0b00010000,
	0b00001000,
	0b00100100,
	0b01000010,
	0b10000001
};

u8 i2c_buff[2];

void send_cmd(u8 cmd) {
	i2c_buff[0] = cmd;
	write_i2c(SID, i2c_buff, 1, false);
}

void write_row(uint8_t y, uint8_t xs) {
	// the row of xs beed to be quirkily transformed
	uint8_t bits = 0;
	for (int x = 0; x < 7; ++x) {
		bits <<= 1;
		bits |= (xs & 1);
		xs  >>= 1;
	}
	bits |= (xs << 7);

	// send x layout to device
	//send2(2 * y, bits);

	i2c_buff[0] = 2*y;
	i2c_buff[1] = bits;
	write_i2c(SID, i2c_buff, 2, false);

}

void setup() {

	//Wire.begin();
	init_serial();
	puts("stm32 i2c bare 8x8 here 3");

	//pu32("I2C1_->CR1", I2C1_->CR2);
	init_i2c();
	//pu32("I2C1_->CR1", I2C1_->CR2);
	send_cmd(0x20 | 1); // turn on oscillator
	send_cmd(0x81); // display on
	send_cmd(0xE0 | 0); // brightness to dimmest (but you should probably set it)

}

void loop() {
	for (int i = 0; i < 8; ++i) {
		u8 row = pattern[i];
		write_row(i, row);
		row = ((row & 0x01) ? 0x80 : 0x00) | (row >> 1); // rotate the row for fun
		pattern[i] = row;
	}
	for(int i=0; i< 60000; i++) nop(); // simple delay
	//delay(100);
}

int main()
{
	setup();
	while(1) loop();



	/*
	   while(1) {
	   static int i = 0;		
	   print("Begin reading attempt 1");
	   char str[10];
	   itoa(i++, str, 10);
	   puts(str);
	   i2c_read(SID, i2c_buff, 1);
	   itoa(i2c_buff[0], str, 10);
	   puts(str);
	   }
	   */
}
