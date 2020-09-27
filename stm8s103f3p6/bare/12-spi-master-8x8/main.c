#include <stm8.h>

#define REG(addr) *(volatile u8*)(addr)

#define I2C_CR1 	REG(0x5200)
#define I2C_CR2 	REG(0x5201)
#define I2C_FREQR 	REG(0x5212)
#define I2C_DR 		REG(0x5216)
#define I2C_SR1 	REG(0x5217)



#define I2C_CR1_PE (1<<0)

#define I2C_CR2_START (1<<0)
#define I2C_CR2_STOP (1<<1)
#define I2C_CR2_ACK (1<<2)

#define I2C_SR1_SB (1<<0)
#define I2C_SR1_RXNE (1<<6)
#define I2C_SR1_TXE (1<<7)

void begin_i2c_trans(u8 addr)
{
	I2C_CR2 |= I2C_CR2_ACK; 
	I2C_CR2 |= I2C_CR2_START; // generate a start condition
	while(!(I2C_SR1 & I2C_SR1_SB));
	I2C_DR = addr;
}

void write_i2c(u8 dat)
{
	while(!(I2C_SR1 &I2C_SR1_TXE));
	I2C_DR = dat;
}

void end_i2c_trans()
{
	I2C_CR2 |= I2C_CR2_STOP;
}

void send_cmd(u8 cmd)
{
	begin_i2c_trans(0x70);
	write_i2c(cmd);
	end_i2c_trans();
}

void main()
{
	// init I2C
	I2C_CR1 |= I2C_CR1_PE; // enable peripheral
	I2C_CR2 |= I2C_CR2_ACK; 
	I2C_FREQR = 0x02; // 2MHz
	I2C_CR2 |= I2C_CR2_START; // generate a start condition

	// init 8x8
	send_cmd(0x20|1); // turn on oscillator
	send_cmd(0x81); // display on
	send_cmd(0xE0 | 0); // brightness dimmest

	begin_i2c_trans(0x70);
	write_i2c(2);
	write_i2c(2);
	end_i2c_trans();

	while(1);
}

