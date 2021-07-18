/*
 * Memory-to-memory must use DMA2
 *
 * The F4 has 2 controllers: DMA1, and DMA2.
 * Each DMA has 8 streams: pathways where memory can flow
 *
 * Further info:
 * https://adammunich.com/stm32-dma-cheat-sheet/
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
//#include <libopencm3/stm32/timer.h>
//#include <libopencm3/stm32/spi.h>
//#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/i2c.h>

#include <string.h>

#include "mal.h"

typedef uint32_t u32;

#define LED PC13


static void myputs(const char *str)
{
	mal_usart_print(str);
	mal_usart_print("\r\n");
}


void nop(void);
void nop(void)
{
	__asm__ volatile ("nop");
}

int main(void)
{
	pin_out(LED);
	mal_usart_init();

	myputs("");
	myputs("=============================");
	myputs("I2C example: master");

	rcc_periph_clock_enable(RCC_I2C1);
	rcc_periph_clock_enable(RCC_GPIOB);
	//i2c_reset(I2C1);
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO6 | GPIO7);
	gpio_set_af(GPIOB, GPIO_AF4, GPIO6 | GPIO7);
	//i2c_peripheral_disable(I2C1);
	//i2c_enable_ack(I2C1);
	i2c_peripheral_enable(I2C1);

	const uint8_t addr = 0x4; // set this to whatever your slave address is
	uint8_t data;
	while(1) {
		i2c_transfer7(I2C1, addr, 0, 0, &data, 1); // read
		//i2c_send_stop(I2C1); // not sure if necessary
		/*
		   i2c_send_start(I2C1);
		   i2c_send_7bit_address(I2C1, slave, I2C_READ);
		   volatile uint8_t v = i2c_get_data(I2C1);
		   i2c_send_stop(I2C1);
		   */
		//mal_delayish(10);
		nop();
		pin_toggle(LED);
	}

}
