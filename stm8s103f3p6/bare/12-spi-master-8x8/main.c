#include <stm8.h>

#define REG(addr) *(volatile u8*)(addr)

#define I2C_CR1 REG(0x5200)
#define I2C_CR2 REG(0x5201)

#define I2C_CR1_PE (1<<0)

#define I2C_CR2_START (1<<0)
#define I2C_CR2_ACK (1<<2)

void main()
{
	gpio_mode_out(PC3);
	gpio_write(PC3, 1);

	gpio_mode_out(PB4);
	gpio_mode_out(PB5);

	I2C_CR1 |= I2C_CR1_PE; // enable peripheral
	I2C_CR2 |= I2C_CR2_ACK; // generate a start condition
	I2C_CR2 |= I2C_CR2_START; // generate a start condition

	/*
	gpio_write(PB4, 0);

	//PORTB->DDR |= (1<< 5); 
	gpio_write(PB5, 0);
*/
	while(1);
}

