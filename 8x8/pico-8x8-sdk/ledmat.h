#pragma once

#include <bitset>

#include "hardware/i2c.h"

#include "i2cmc.h"


class LedMat {
	public:
		LedMat(I2C& i2c);
		void set(uint8_t row, uint8_t col, bool on);
		void show();
	private:
		i2c_inst_t* m_i2c_inst;
		void send_cmd(uint8_t cmd);
		uint8_t m_addr = 0x70; // slave ID of device
		std::bitset<8> m_grid[8];
};

