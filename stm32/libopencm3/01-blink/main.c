#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#define PORT_LED1 GPIOC
#define PIN_LED1 GPIO13

//#define LITTLE_BIT 50000UL
int main(void) {
	//rcc_periph_clock_enable(RCC_LED1);
	gpio_set_mode(PORT_LED1, GPIO_MODE_OUTPUT_2_MHZ, 
			GPIO_CNF_OUTPUT_PUSHPULL, PIN_LED1);
	gpio_set(PORT_LED1, PIN_LED1);
	while(1) {
		/* wait a little bit */
		for (int i = 0; i < 500000UL; i++) {
			__asm__("nop");
		}
		gpio_toggle(PORT_LED1, PIN_LED1);
	}
}
