#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/spi.h>

#include "mal.h"


void mal_max7219_init(void)
{
	mal_spi_init_std();

	mal_max7219_tfr(0x0F, 0x00);
	mal_max7219_tfr(0x09, 0xFF); // Enable mode B
	mal_max7219_tfr(0x0A, 0x0F); // set intensity (page 9)
	mal_max7219_tfr(0x0B, 0x07); // use all pins
	mal_max7219_tfr(0x0C, 0x01); // Turn on chip
}

void mal_max7219_show_count(int count)
{
	uint32_t num = count;
	for (uint8_t i = 0; i < 8; ++i)
	{
		uint8_t c = num % 10;
		num /= 10;
		uint8_t sep = 0; // thousands separator

		// add in thousands separators
		if ((i > 0) && (i % 3 == 0))
		{
			sep = 1 << 7;
		}

		// blank if end of number
		if ((c == 0) && (num == 0) && (i > 0))
		{
			sep = 0;
			c = 0b1111;
		}

		c |= sep;

		mal_max7219_tfr(i + 1, c);
		//delay(1);
	}

}

void mal_max7219_tfr(uint8_t address, uint8_t value) 
{
	gpio_clear(GPIOB, GPIO12);
	spi_xfer(SPI2, address); // seems to be roughly equiv of spi_send(SPI2, address); spi_read(SPI2);
	spi_xfer(SPI2, value); // seems to be roughly equiv of spi_send(SPI2, value); spi_read(SPI2);
	gpio_set(GPIOB, GPIO12);
}

void mal_spi_init_std(void)
{
	rcc_periph_clock_enable(RCC_SPI2);
	rcc_periph_clock_enable(RCC_GPIOB);
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO13 | GPIO14 | GPIO15);
	gpio_set_af(GPIOB, GPIO_AF5, GPIO13 | GPIO14 | GPIO15);
	spi_init_master(SPI2, SPI_CR1_BAUDRATE_FPCLK_DIV_16, 
		SPI_CR1_CPOL, SPI_CR1_CPHA, SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);
	//spi_enable_ss_output(SPI2); /* Required, see NSS, 25.3.1 section. */
	gpio_mode_setup(GPIOB,  GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12); // chip select
	gpio_set(GPIOB, GPIO12);
	spi_enable(SPI2);
}

pin_t pb12 = { GPIOB, GPIO12, RCC_GPIOB };
pin_t pc13 = { GPIOC, GPIO13, RCC_GPIOC };
pin_t pc14 = { GPIOC, GPIO14, RCC_GPIOC };

void pin_out(pin_t* pin)
{
	rcc_periph_clock_enable(pin->rcc);
	//gpio_set_mode(pin->port, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, pin->num);
	//gpio_set_mode(pin->port, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_OPENDRAIN, pin->num);
	gpio_mode_setup(pin->port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, pin->num);
}

void pin_toggle(pin_t* pin)
{
	gpio_toggle(pin->port, pin->num);
}

void pin_high(pin_t* pin)
{
	gpio_set(pin->port, pin->num);
}
void pin_low(pin_t* pin)
{
	gpio_clear(pin->port, pin->num);
}



timer_t timer2 = { TIM2, RCC_TIM2, NVIC_TIM2_IRQ};



void mal_timer_init(timer_t* timer, double freq)
{
	rcc_periph_clock_enable(timer->rcc);
	uint32_t clk_freq = rcc_get_timer_clk_freq(timer->peri);
	timer_set_prescaler(TIM2, clk_freq/1000000-1); // set the CK_CNT (clock counter) freq to 1MHz
	double period = 1000000.0/freq -1.0;
	timer_set_period(timer->peri, period); // twice a second. Equivalent to TIM_ARR(TIM2) = period
	timer_generate_event(timer->peri, TIM_EGR_UG); // send an update to reset timer and apply settings
	//timer_generate_event(TIM2, TIM_EGR_UG | TIM_EGR_TG); // equiv: TIM_EGR(TIM2) |= (TIM_EGR_UG | TIM_EGR_UG)
	//timer_enable_update_event(TIM2); // equiv: TIM_CR1(TIM2) &= ~TIM_CR1_UDIS
	timer_enable_counter(timer->peri); // equiv: TIM_CR1(TIM2) |= TIM_CR1_CEN
	timer_enable_irq(TIM2, TIM_DIER_UIE); // equiv: TIM_DIER(TIM2) |= TIM_DIER_UIE
	nvic_enable_irq(timer->irq);
}



void mal_usart_init(void)
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
}


void mal_usart_print(const char* str)
{
	while(*str) usart_send_blocking(USART1, *str++);
}

