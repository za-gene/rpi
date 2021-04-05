#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/spi.h"
#include "hardware/gpio.h"
//#include <type_traits>
#include <bitset>


#include "i2cmc.h"


#define BTN 14 // GPIO number, not physical pin
#define LED 25 // GPIO of built-in LED



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

void LedMat::send_cmd(uint8_t cmd)
{
	i2c_write_blocking(m_i2c_inst, m_addr, &cmd, 1, false);

}


LedMat::LedMat(I2C& i2c) : m_i2c_inst(i2c.get())
{
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

I2C i2c(i2c1, 26);
LedMat ledmat(i2c);

static uint8_t  pattern1[] = { 
	0b1000'0001,
	0b0100'0010,
	0b0010'0100,
	0b0001'0000,

	0b0000'1000,
	0b0010'0100,
	0b0100'0010,
	0b1000'0001
};

static uint8_t  pattern2[] = { // the letter P, with some orientation frills
	0b1111'0001,
	0b1000'1000,
	0b1000'1000,
	0b1111'0000,

	0b1000'0000,
	0b1000'0000,
	0b1000'0001,
	0b1000'0010
};

int main() 
{
	stdio_init_all();

	//initalise display
	for(int r=0; r<8; r++) {
		uint8_t row = pattern2[r];
		for(int c=0; c<8; c++) {
			ledmat.set(r,c, row >> 7);
			row <<=1;
		}
	}
	ledmat.show();


	gpio_init(BTN);
	gpio_set_dir(BTN, GPIO_IN);
	gpio_pull_up(BTN);

	gpio_init(LED);
	gpio_set_dir(LED, GPIO_OUT);

	int i = 0;
	for(;;) {
		printf("Hello number %d\n", i++);
		gpio_put(LED, 1);
		sleep_ms(100);
		gpio_put(LED, 0);
		sleep_ms(1000);		
	}

	return 0;
}

