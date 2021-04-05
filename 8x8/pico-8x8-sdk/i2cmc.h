#pragma once

#include "hardware/gpio.h"
#include "hardware/i2c.h"

class I2C {
	public:
		I2C(i2c_inst_t* i2c = i2c0, uint sda = 4, uint baudrate = 100'000);
		constexpr i2c_inst_t* get();
	private:
		i2c_inst_t* m_i2c;
		uint m_sda;

};

