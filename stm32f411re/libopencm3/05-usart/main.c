/*
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/usart.h>
*/

#include "mal.h"

typedef uint32_t u32;

int main(void)
{

	mal_usart_init();

	char msg[] = "I will echo everything you type...\r\n";
	mal_usart_print(msg);

	while (1)
	{
		uint16_t c = usart_recv_blocking(USART1);
		usart_send_blocking(USART1, c);
	}
}
