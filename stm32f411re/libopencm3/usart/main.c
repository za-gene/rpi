#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/usart.h>

typedef uint32_t u32;


int main(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_USART1);
	gpio_mode_setup(GPIOA, GPIO_MODE_AF,GPIO_PUPD_NONE, GPIO9); // TX
	gpio_set_af(GPIOA, GPIO_AF7, GPIO9);
	gpio_mode_setup(GPIOA, GPIO_MODE_AF,GPIO_PUPD_NONE, GPIO10); // RX
	gpio_set_af(GPIOA, GPIO_AF7, GPIO10);
	usart_set_baudrate(USART1, 115200);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_mode(USART1, USART_MODE_TX_RX);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
	usart_enable(USART1);

	char msg[] = "I will echo everything you type...\r\n";
	char *str = msg;
	while(*str) usart_send_blocking(USART1, *str++);

	while (1)
	{
		uint16_t c = usart_recv_blocking(USART1);
		usart_send_blocking(USART1, c);
	}
}
