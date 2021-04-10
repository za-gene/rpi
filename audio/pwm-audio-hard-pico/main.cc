/* iffy: blinkt doesn't seem to work with this project */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "hardware/irq.h"

#include "track.h"

#define	PIN_SCK		2
#define	PIN_MOSI	3
#define PIN_MISO 	4
#define	PIN_CS 		5

using u16 = uint16_t;

void my_pwm_wrap_isr();

class digiout {
	public:
		digiout(uint8_t pin);
		void put(bool value);
		void toggle();
	private:
		bool _on = false;
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
	_on = value !=0;
	gpio_put(_pin, _on);
}

void digiout::toggle()
{
	put(!_on);
}

class pwm {
	public:
		pwm();
		void set_level(uint16_t vol);
	private:
		uint _slice_num;
};

digiout pin16(16);
digiout pin17(17);



const int top = 1023;
const int f_pwm = 32000;
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
	float scale = (top+1) * f_pwm;
	float divider = f_sys / scale;  
	//divider = 1.0;
	pwm_set_clkdiv(_slice_num, divider); // pwm clock should now be running at 1MHz

	pwm_set_wrap(_slice_num, top);
	set_level(2000) ;
	pwm_set_enabled(_slice_num, true); // let's go!
	pwm_set_chan_level(_slice_num, PWM_CHAN_B, 200); // GPIO15 is a trigger

	pwm_clear_irq(_slice_num);
	pwm_set_irq_enabled(_slice_num, true);
	//irq_add_shared_handler(PWM_IRQ_WRAP, my_pwm_wrap_isr, PICO_SHARED_IRQ_HANDLER_DEFAULT_ORDER_PRIORITY);
	irq_set_exclusive_handler(PWM_IRQ_WRAP, my_pwm_wrap_isr);
	irq_set_enabled(PWM_IRQ_WRAP, true);
}

void pwm::set_level(uint16_t vol)
{
	pwm_set_chan_level(_slice_num, PWM_CHAN_A, vol); 
	//pwm_set_enabled(_slice_num, true); // let's go!
}



pwm a_pwm;

void my_pwm_wrap_isr()
{
	const int time_scale = f_pwm / 8000;
	const int vol_scale = (top+1)/256;
	static int i = 0;
	u16 v = track_raw[i/time_scale];
	i++;
	if(i == time_scale * sizeof(track_raw)) i = 0;
	a_pwm.set_level(v*vol_scale);

	pin16.toggle();
	//irq_clear(PWM_IRQ_WRAP);
	pwm_clear_irq(7); // pwm gpio 14 and 15 on slice 7
}

int main() 
{

	spi_init(spi0, 4'000'000);
	spi_set_slave(spi0, true);
	spi_set_format(spi0, 16, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST); 
	gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
	gpio_set_function(PIN_CS,   GPIO_FUNC_SPI);
	gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
	gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

	while(1); // actual playing of track is handled by my_pwm_wrap_isr()

	return 0;
}

