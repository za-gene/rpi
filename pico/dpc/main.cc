/* iffy: blinkt doesn't seem to work with this project */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#include "blinkt.h"

#define	PIN_SCK		2
#define	PIN_MOSI	3
#define PIN_MISO 	4
#define	PIN_CS 		5

class digiout {
	public:
		digiout(uint8_t pin);
		void put(bool value);
	private:
		uint8_t _pin;
};

digiout::digiout(uint8_t pin)
{
	_pin = pin;
     	gpio_init(pin);
        gpio_set_dir(pin, GPIO_OUT);
}

void digiout::put(bool value)
{
	gpio_put(_pin, value != 0);
}

class pwm {
	public:
		pwm();
		void set_level(uint16_t vol);
	private:
		uint _slice_num;
};

digiout pin17(17);

pwm::pwm()
{
	// the PWM channel number is documents in the datasheet, s4.5.2 p535
	// GPIO14 is 7A, 15 is 7B
	gpio_set_function(14, GPIO_FUNC_PWM); 
	gpio_set_function(15, GPIO_FUNC_PWM); 
	_slice_num = pwm_gpio_to_slice_num(14); 
	// assert that they're both on slice 7, as expected
	bool same_slice = _slice_num == 7 && pwm_gpio_to_slice_num(15) == 7;
	pin17.put(same_slice);


	// run the clock at 44.1kHz
	uint32_t f_sys = clock_get_hz(clk_sys); // typically 125'000'000 Hz
	const int top = 4095;
	const int f_pwm = 8000;
	float scale = (top+1) * f_pwm;
	float divider = f_sys / scale;  
	//divider = 1.0;
	pwm_set_clkdiv(_slice_num, divider); // pwm clock should now be running at 1MHz

	pwm_set_wrap(_slice_num, top);
	set_level(2000) ;
	pwm_set_enabled(_slice_num, true); // let's go!
	pwm_set_chan_level(_slice_num, PWM_CHAN_B, 200); // GPIO15 is a trigger

}

void pwm::set_level(uint16_t vol)
{
	pwm_set_chan_level(_slice_num, PWM_CHAN_A, vol); 
	//pwm_set_enabled(_slice_num, true); // let's go!
}



pwm a_pwm;
digiout pin16(16);

int main() 
{
	//stdio_init_all();
	//puts("dpc started");
	//while(1) putchar('.');

	pin16.put(1); // simply confirms that pico is running
	/*
	blinkt_init(16, 17);
	sleep_ms(100);
	blinkt_show();
	sleep_ms(100);
	blinkt_set_pixel_colour(1, 10, 0, 0);
	sleep_ms(100);
	blinkt_show();
	*/
	//while(1);

	spi_init(spi0, 4'000'000);
	spi_set_slave(spi0, true);
	spi_set_format(spi0, 16, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST); 
	gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
	gpio_set_function(PIN_CS,   GPIO_FUNC_SPI);
	gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
	gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

	//pwm0.set_level(4096/2);
	//while(1);

	for(;;) {
		uint16_t rx, tx = 42;
		spi_read16_blocking(spi0, tx, &rx, 1);
		rx &= 0xfff; // 12-bit value
		//printf("Received %d\n", rx);
		a_pwm.set_level(rx);
	}

	/*
	gpio_init(BTN);
	gpio_set_dir(BTN, GPIO_IN);
	gpio_pull_up(BTN);

	gpio_init(LED);
	gpio_set_dir(LED, GPIO_OUT);

	for(;;) {
		gpio_put(LED, 1);
		sleep_ms(100);
		gpio_put(LED, 0);
		sleep_ms(1000);
	}
	*/

	return 0;
}

