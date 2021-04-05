#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
//#include "hardware/spi.h"
#include "hardware/gpio.h"
#include <type_traits>


#define BTN 14 // GPIO number, not physical pin
#define LED 25 // GPIO of built-in LED


class I2C {
	public:
		I2C(i2c_inst_t* i2c = i2c0, uint sda = 4, uint baudrate = 100'000);
		constexpr i2c_inst_t* get();
	private:
		i2c_inst_t* m_i2c;
		uint m_sda;

};


I2C::I2C(i2c_inst_t* i2c, uint sda, uint baudrate) : m_i2c(i2c), m_sda(sda)
{
	i2c_init(i2c, baudrate);
	gpio_set_function(sda, GPIO_FUNC_I2C);
	gpio_pull_up(sda);
	// sda is next pin
	gpio_set_function(sda+1, GPIO_FUNC_I2C);
	gpio_pull_up(sda+1);

}

constexpr i2c_inst_t* I2C::get()
{
	return m_i2c;
}


/*
   i2c_inst_t* i2c_config(i2c_inst_t i2c_inst = i2c0, uint sda = 4, uint baudrate = 100'000)
   {

   }
   */

class LedMat {
	public:
		LedMat(I2C& i2c);
	private:
		i2c_inst_t* m_i2c_inst;
		void send_cmd(uint8_t cmd);
		uint8_t m_addr = 0x70; // slave ID of device
};

void LedMat::send_cmd(uint8_t cmd)
{
	i2c_write_blocking(m_i2c_inst, m_addr, &cmd, 1, false);

}


LedMat::LedMat(I2C& i2c) : m_i2c_inst(i2c.get())
{
	send_cmd(0x20 | 1); // turn on oscillator
	send_cmd(0x81); // display on
	send_cmd(0xE0 | 0); // brightness to dimmest (but you should probably set it)

	for(int r = 0; r<8; r++) {
	uint8_t data[2];
	data[0] = r*2;
	data[1] = 3;
	i2c_write_blocking(m_i2c_inst, m_addr, data, 2, false);
	}
}



I2C i2c(i2c1, 26);
LedMat ledmat(i2c);

/*
   void i2c_init_mc(const uint&& gpio)
   {
//static_assert(((gpio <=20) && (gpio%2 == 0)) || gpio == 26, "invalid i2c sda pin");
}
*/

int main() 
{
	stdio_init_all();

	//i2c_init_mc(26);

	gpio_init(BTN);
	gpio_set_dir(BTN, GPIO_IN);
	gpio_pull_up(BTN);
	// gpio_get() gets state of pin

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

