#include <basal.h>
#include <timers.h>
#include <gpio.h>
//#include <mini_uart.h>
#include <interrupts.h>



#define SYSTIMERCLO REG(0x3F003004)




#define TIME_INT 1000000        		// in microsec

#if 0
const int pin = 19;
#else
const int pin = 26;
#endif



void blink_led()
{
	gpio_toggle(pin);
	ARM_TIMER_CLI = 0; // clear the timer interrupt
}

void kernel_main ( void )
{
	gpio_sel(pin, OUTPUT);

	ARM_TIMER_CTL = 0x003E0000;	// 0x3E is the reset for the counter

	ARM_TIMER_DIV = 0x000000F9;	// dividing APB_CLK by 0xF9 + 1 (250) -> 1 MHz (this signal is timer_clk)
	u32 system_clock_freq = 250000000/(ARM_TIMER_DIV+1);
	u32 freq = 400*2;
	u32 timer_counter = system_clock_freq/freq; // not worrying about decr
	if(freq * timer_counter == system_clock_freq) timer_counter--; // adjust for slight exactitude
	ARM_TIMER_LOD = timer_counter;	// 1000000 is equal to 1 second
	ARM_TIMER_RLD = timer_counter;	// RLD is copied tO LOD when it reaches 0
	ARM_TIMER_CLI = 0;				// writing here clears the interrupt (write only)
	ARM_TIMER_CTL = 0x003E00A2;	// 23bit counting mode, no timer_clk prescaling, enabling interrupts and the timer
	IRQ_ENABLE_BASIC = 1;			// enabling interrupts

	set_irq_handler(blink_led);
	enable_irq();
}
