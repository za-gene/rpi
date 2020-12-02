#include <basal.h>
#include <timers.h>
#include <gpio.h>
#include <interrupts.h>



//#define SYSTIMERCLO REG(0x3F003004)




#if 0
const int pin = 19;
#else
const int pin = 26;
#endif


void init_arm_timer(u32 freq)
{
	//ARM_TIMER_CTL = 0x003E0000;	// 0x3E is the reset for the counter

	//ARM_TIMER_DIV = 0x000000F9;	// dividing APB_CLK by 0xF9 + 1 (250) -> 1 MHz (this signal is timer_clk)
	//ARM_TIMER_DIV = 0x3E;	// dividing APB_CLK by 0xF9 + 1 (250) -> 1 MHz (this signal is timer_clk)
	//ARM_TIMER_DIV = 10;	// dividing APB_CLK by 0xF9 + 1 (250) -> 1 MHz (this signal is timer_clk)
	u32 timer_clock_freq = apb_clock_freq/(ARM_TIMER_DIV+1);
	u32 reload_value = timer_clock_freq/freq; // not worrying about decr
	if(freq * reload_value == timer_clock_freq) reload_value--; // adjust for slight exactitude
	//ARM_TIMER_LOD = timer_counter;	// 1000000 is equal to 1 second
	ARM_TIMER_RLD = reload_value;	// RLD is copied tO LOD when it reaches 0
	ARM_TIMER_CLI = 0;				// writing here clears the interrupt (write only)
	//ARM_TIMER_CTL = 0x003E00A2;	// 23bit counting mode, no timer_clk prescaling, enabling interrupts and the timer
	ARM_TIMER_CTL |= (1<<7) // enable timer
		| (1<<5); // enable timer interrupts
}


void enable_arm_timer_irq()
{
	IRQ_ENABLE_BASIC |= (1<<0); // enable ARM timer IRQ
}

void blink_led()
{
	gpio_toggle(pin);
	ARM_TIMER_CLI = 0; // clear the timer interrupt
}



void kernel_main ( void )
{
	gpio_sel(pin, OUTPUT);

	u32 freq = 10000*2;
	init_arm_timer(freq);

	enable_arm_timer_irq();
	set_irq_handler(blink_led);
	enable_irq();
}
