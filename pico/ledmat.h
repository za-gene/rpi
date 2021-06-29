#pragma once

/**
 * Released into the Public Domain.
 *
 * Adafruit I2C LED Matrix control.
 *
 * Assumes the top of the matrix has the pins uppermost.
 *
 * Co-ordinate convention:
 * Rows are set from top to bottom: 0 to 7 inc.
 * Columns are set left to right: 0 to 7 inc.
 *
 */




#include <pico/stdlib.h>
#include "hardware/i2c.h"

#include <bitset>

//#include "i2cmc.h"


class LedMat {
	public:
		LedMat();
		void set(uint8_t row, uint8_t col, bool on);
		void show();
	private:
		i2c_inst_t* m_i2c_inst;
		void send_cmd(uint8_t cmd);
		uint8_t m_addr = 0x70; // slave ID of device
		std::bitset<8> m_grid[8];
};

