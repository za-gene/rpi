#include <i2c.h>



/** begin I2C transaction */
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


/** begin I2C transaction 
2021-02-03 confirmed working with SSD1306 and 8x8
*/
void end_i2c_trans()
{
#if 0
	for(u8 i = 0; i< 100; i++) nop();
	I2C_CR2 |= I2C_CR2_STOP;
#else
	while (!((I2C_SR1 & (I2C_SR1_TXE | I2C_SR1_BTF)) == (I2C_SR1_TXE | I2C_SR1_BTF)));
	I2C_CR2 |= I2C_CR2_STOP;
	while (I2C_CR2 & I2C_CR2_STOP);
#endif
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

/** write a single byte to I2C. NB that who must be within an I2C transaction 
for it to work 
E.g.
begin_i2c_trans(sid);
write_i2c_byte(0x42);
...
end_ic2_trans();
*/

void write_i2c_byte(uint8_t dat)
{
	while (!(I2C_SR1 & I2C_SR1_TXE));
	I2C_DR = dat;
}
