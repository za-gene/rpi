// attempt to convert arduino library to bare metal

#include <stm8.h>
#include <millis.h>
#include <i2c.h>



#define SID 0x70 // Slave ID




static void end_i2c_trans(void)
{
	while (!((I2C_SR1 & (I2C_SR1_TXE | I2C_SR1_BTF)) == (I2C_SR1_TXE | I2C_SR1_BTF)));

	I2C_CR2 |= I2C_CR2_STOP;
	while (I2C_CR2 & I2C_CR2_STOP);
}

void write_i2c_byte(uint8_t dat)
{
	while (!(I2C_SR1 & I2C_SR1_TXE));
	I2C_DR = dat;
}



void send_cmd(u8 cmd) {
	begin_i2c_trans(SID);
	write_i2c_byte(cmd);
	end_i2c_trans();
}


void write_row(uint8_t y, uint8_t xs) {
	// the row of xs needs to be quirkily transformed
	uint8_t bits = 0;
	for (int x = 0; x < 7; ++x) {
		bits <<= 1;
		bits |= (xs & 1);
		xs  >>= 1;
	}
	bits |= (xs << 7);

	// send x layout to device
	begin_i2c_trans(SID);
	write_i2c_byte(2*y);
	write_i2c_byte(bits);
	end_i2c_trans();

}




static uint8_t  pattern[] = { 
	//0b11111111,
	0b10000001,
	0b01000010,
	0b00100100,
	0b00010000,
	0b00001000,
	0b00100100,
	0b01000010,
	0b10000001
};

void main() {
	//Serial_begin(115200);
	init_millis();
	init_i2c();
	send_cmd(0x20 | 1); // turn on oscillator
	send_cmd(0x81); // display on
	send_cmd(0xE0 | 0); // brightness to dimmest (but you should probably set it)

	//pattern[0] = 0b11111111;

	while(1) {
		for (int i = 0; i < 8; ++i) {
			u8 row = pattern[i];
			write_row(i, row);
			row = ((row & 0x01) ? 0x80 : 0x00) | (row >> 1); // rotate the row for fun
			pattern[i] = row;
		}
		delay_millis(100);
	}
}
