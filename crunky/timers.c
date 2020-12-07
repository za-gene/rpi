/*
 * Copyright (C) 2018 bzt (bztsrc@github)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include <basal.h>
#include <timers.h>
#include <gpio.h>

#define SYSTMR_LO        REG(PBASE+0x00003004)
#define SYSTMR_HI        REG(PBASE+0x00003008)

/**
 * Wait N CPU cycles (ARM CPU only)
 */
void __attribute__((optimize("O0"))) wait_cycles(unsigned int n)
{
    while(n-- > 0);
    //if(n) while(n--) { asm volatile("nop"); }
}

/**
 * Wait N microsec (ARM CPU only)
 */

/*
void wait_msec(unsigned int n)
{
    register unsigned long f, t, r;
    // get the current counter frequency
    asm volatile ("mrs %0, cntfrq_el0" : "=r"(f));
    // read the current counter
    asm volatile ("mrs %0, cntpct_el0" : "=r"(t));
    // calculate expire value for counter
    t+=((f/1000)*n)/1000;
    do{asm volatile ("mrs %0, cntpct_el0" : "=r"(r));}while(r<t);
}
*/

/**
 * Get System Timer's counter
 */
uint64_t get_system_timer()
{
    u32 h=-1, l;
    // we must read MMIO area as two separate 32 bit reads
    h=SYSTMR_HI;
    l=SYSTMR_LO;
    // we have to repeat it if high word changed during read
    if(h!=SYSTMR_HI) {
        h=SYSTMR_HI;
        l=SYSTMR_LO;
    }
    uint64_t res = h;
    res <<= 32;
    res += l;
    // compose long int value
    return res;
}

/**
 * Wait N microsec (with BCM System Timer)
 */
void delay_us(uint64_t n)
{
    uint64_t us=get_system_timer();
    // we must check if it's non-zero, because qemu does not emulate
    // system timer, and returning constant zero would mean infinite loop
    if(us) while(get_system_timer() -us < n);
}

void delay_ms(int ms)
{
        for(int i=0; i<ms; ++i)
                delay_us(1000);
}

void delay_s(int secs)
{
        for(int i=0; i<secs; ++i)
                delay_ms(1000);
}

void init_arm_timer(u32 freq)
{
	u32 timer_clock_freq = apb_clock_freq/(ARM_TIMER_DIV+1);
	u32 reload_value = timer_clock_freq/freq; // not worrying about decr
	if(freq * reload_value == timer_clock_freq) reload_value--; // adjust for slight exactitude
	ARM_TIMER_RLD = reload_value;	// RLD is copied tO LOD when it reaches 0
	ARM_TIMER_CLI = 0;				// writing here clears the interrupt (write only)
	ARM_TIMER_CTL |= (1<<7) // enable timer
		| (1<<5); // enable timer interrupts
}
