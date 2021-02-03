#include <i2c.h>



void begin_i2c_trans(uint8_t slave_id)
{

	I2C_CR2 |= I2C_CR2_ACK;  // set ACK
	I2C_CR2 |= I2C_CR2_START;  // send start sequence
	while (!(I2C_SR1 & I2C_SR1_SB)); // EV5
	I2C_DR = (slave_id << 1); 

	// EV6 ADDR=1, cleared by reading SR1 register, then SR3
	while (!(I2C_SR1 & I2C_SR1_ADDR));
	I2C_SR3;   // read SR3 to clear ADDR event bit

}


/** set it to a 100kb standard rate, "probably" */
void init_i2c() 
{
	I2C_CR1 &= ~I2C_CR1_PE; // disable I2C
	//uint32_t OutputClockFrequencyHz = I2C_MAX_STANDARD_FREQ;
	//uint8_t InputClockFrequencyMHz = 2; // 16;
	//InputClockFrequencyMHz  = 2;
	I2C_FREQR = 2;
	I2C_TRISER = 2 + 1; // max rise time. Doesn't seem to like I2C_TRISER = I2C_FREQR + 1;

	//uint16_t speed = (uint16_t)((InputClockFrequencyMHz * 1000000) / (OutputClockFrequencyHz / 2));
	//speed = 4;
	I2C_CCRL = 4;
	I2C_CCRH = 0;


	I2C_CR1 |= I2C_CR1_PE; // enable I2C
}
