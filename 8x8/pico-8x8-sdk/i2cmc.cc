#include "i2cmc.h"


I2C::I2C(i2c_inst_t* i2c, uint sda, uint baudrate) : m_i2c(i2c), m_sda(sda)
{
	i2c_init(i2c, baudrate);
	gpio_set_function(sda, GPIO_FUNC_I2C);
	gpio_pull_up(sda);
	// sda is next pin
	gpio_set_function(sda+1, GPIO_FUNC_I2C);
	gpio_pull_up(sda+1);

}

i2c_inst_t* const I2C::get()
{
	return m_i2c;
}


