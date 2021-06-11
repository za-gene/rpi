#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/usart.h>

typedef uint32_t u32;


int main(void)
{
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);
	gpio_set(GPIOC, GPIO13);

	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_USART1);
	gpio_mode_setup(GPIOA, GPIO_MODE_AF,GPIO_PUPD_NONE, GPIO9); // TX
	gpio_set_af(GPIOA, GPIO_AF7, GPIO9);
	usart_set_baudrate(USART1, 115200);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_mode(USART1, USART_MODE_TX);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
	usart_enable(USART1);

#define LITTLE_BIT 1000000
	while (1)
	{
		/* wait a little bit */
		for (int i = 0; i < LITTLE_BIT; i++)
		{
			__asm__("nop");
		}
		gpio_toggle(GPIOC, GPIO13);
		usart_send_blocking(USART1, '.');
	}
}
