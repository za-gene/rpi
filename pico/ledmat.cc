#include "hardware/gpio.h"

#include "ledmat.h"


void LedMat::send_cmd(uint8_t cmd)
{
	i2c_write_blocking(m_i2c_inst, m_addr, &cmd, 1, false);

}


LedMat::LedMat()
{
	// init i2c
	m_i2c_inst = i2c0;
	const auto baudrate = 100'000;
    const auto sda = 4;
	i2c_init(m_i2c_inst, baudrate);
	gpio_set_function(sda, GPIO_FUNC_I2C);
	gpio_pull_up(sda);
	// sda is next pin
	gpio_set_function(sda+1, GPIO_FUNC_I2C);
	gpio_pull_up(sda+1);
	
	send_cmd(0x20 | 1); // turn on oscillator
	show(); // clear out any display that is there
	send_cmd(0x81); // display on
	send_cmd(0xE0 | 0); // brightness to dimmest (but you should probably set it)

}


void LedMat::set(uint8_t row, uint8_t col, bool on)
{
	// the LED has a strange co-ordinate system
	int pos1 = 7 - col; // actually a "row"
	int pos2 = row -1;
	if(pos2 == -1) pos2 = 7;
	m_grid[pos1].set(pos2, on);
}

void LedMat::show()
{
	for(int r = 0; r<8; r++) {
		uint8_t data[2];
		data[0] = r*2;
		data[1] = m_grid[r].to_ulong();
		i2c_write_blocking(m_i2c_inst, m_addr, data, 2, false);
	}

}

