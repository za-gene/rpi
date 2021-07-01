#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/spi.h>

typedef uint32_t u32;

//#define GPIOn GPIO13

typedef struct {
	uint32_t port;
	int num;
	//rcc_periph_clken rcc;
	//rcc_periph_clken rcc;
	int rcc;
} pin_t;


pin_t pb12 = { GPIOB, GPIO12, RCC_GPIOB };
pin_t pc13 = { GPIOC, GPIO13, RCC_GPIOC };
pin_t pc14 = { GPIOC, GPIO14, RCC_GPIOC };
#define PB12 &pb12
#define PC13 &pc13
#define PC14 &pc14

void pin_toggle(pin_t* pin);
void pin_out(pin_t* pin);



void pin_out(pin_t* pin)
{
	rcc_periph_clock_enable(pin->rcc);
	gpio_set_mode(pin->port, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, pin->num);
	//gpio_set_mode(pin->port, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_OPENDRAIN, pin->num);
}

void pin_toggle(pin_t* pin)
{
	gpio_toggle(pin->port, pin->num);
}

void pin_high(pin_t* pin);
void pin_high(pin_t* pin)
{
	gpio_set(pin->port, pin->num);
}
void pin_low(pin_t* pin);
void pin_low(pin_t* pin)
{
	gpio_clear(pin->port, pin->num);
}




void mal_max7219_init(void);
void mal_max7219_show_count(int count);
void mal_max7219_tfr(uint8_t address, uint8_t value);

void mal_spi_init_std(void);


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
	// GPIO14 MISO ignored for now
	rcc_periph_clock_enable(RCC_SPI2);
	rcc_periph_clock_enable(RCC_GPIOB);
	//gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO13 | GPIO14 | GPIO15);
	//gpio_set_af(GPIOB, GPIO_AF5, GPIO13 | GPIO14 | GPIO15);
	//gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, GPIO13 | GPIO15);
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO13 | GPIO15);
	spi_init_master(SPI2, SPI_CR1_BAUDRATE_FPCLK_DIV_256, SPI_CR1_CPOL, SPI_CR1_CPHA, 
			SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);
	//spi_enable_ss_output(SPI2); /* Required, see NSS, 25.3.1 section. */
	pin_out(PB12); // chip select
	pin_high(PB12);
	//gpio_mode_setup(GPIOB,  GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12); // chip select
	spi_enable(SPI2);
}

void mcp4921_write(uint16_t value);

void mcp4921_write(uint16_t value)
{
	if(value>4095) value=4095;
	value |= 0b0011000000000000;
	pin_low(PB12);
	spi_xfer(SPI2, value>>8);
	spi_xfer(SPI2, value & 0xFF);
	pin_high(PB12);
}




// shut the compiler up about missing protypes
void delay(int n);

void delay(int n)
{
	for (int j = 0; j < n; j++)
	{
		for (int i = 0; i < 1000; i++)
		{
			__asm__("nop");
		}
	}
}


double frame_rate = 44100.0; //hz 
volatile float y =0, dy;

void do_periodic(void);

void do_periodic()
{
	y = y + dy;
	mcp4921_write(y);
	if(y>=4095) y = 0;
	pin_toggle(PC14);
}

void tim2_isr(void) // the standard ISR name for TIM2
{
	timer_clear_flag(TIM2, TIM_SR_UIF); // hmmm, seems to matter that it's at the top
	do_periodic();
}

void timer_setup(void);
void timer_setup(void)
{
	// trigger the isr at a given frame rate frequency
	rcc_periph_clock_enable(RCC_TIM2);
	u32 clk_freq = rcc_get_timer_clk_freq(TIM2);
	timer_set_prescaler(TIM2, clk_freq/1000000-1); // set the CK_CNT (clock counter) freq to 1MHz
	double frame_period = 1000000.0/frame_rate -1.0;
	timer_set_period(TIM2, frame_period); // Equivalent to TIM_ARR(TIM2) = period
	timer_generate_event(TIM2, TIM_EGR_UG); // send an update to reset timer and apply settings
	//timer_generate_event(TIM2, TIM_EGR_UG | TIM_EGR_TG); // equiv: TIM_EGR(TIM2) |= (TIM_EGR_UG | TIM_EGR_UG)
	//timer_enable_update_event(TIM2); // equiv: TIM_CR1(TIM2) &= ~TIM_CR1_UDIS
	timer_enable_counter(TIM2); // equiv: TIM_CR1(TIM2) |= TIM_CR1_CEN
	timer_enable_irq(TIM2, TIM_DIER_UIE); // equiv: TIM_DIER(TIM2) |= TIM_DIER_UIE
	nvic_enable_irq(NVIC_TIM2_IRQ);
}

int main(void)
{
	pin_out(PC13);
	pin_out(PC14);


	double wave_freq = 440.0; // hz
	double wave_max = 4095.0;
	dy = wave_max * wave_freq / frame_rate;
	mal_spi_init_std();

	// Set up the builtin LED on PC13
	//rcc_periph_clock_enable(RCC_GPIOC);
	//gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
	//gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIOn);

	//timer_setup();

	//mal_max7219_init();
	//u32 count = 0;
	while(1) {
		do_periodic();
		//mal_max7219_show_count(count++);
		delay(0);
	}
}
