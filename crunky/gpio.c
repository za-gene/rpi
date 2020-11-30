// released into the public domain

#include <gpio.h>
#include <delays.h>
//#include <utils.h>

typedef unsigned int u32; // irrespective of architecture
typedef  unsigned long uintptr; // architecture-dependent

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int);


#define put32 PUT32
#define get32 GET32

void gpio_sel(int bcm_pin, int mode)
{
        uintptr gpfsel = (bcm_pin/10)*4+ MMIO_BASE+ 0x200000;
        u32 value = get32(gpfsel);
        u32 shift = (bcm_pin%10)*3;
        value &= ~(0b111 << shift); // clear the bits
        value |= (mode << shift); // set to desired mode
        put32(gpfsel, value);

	// enable the pin
	// this is the equivalent of gpio_pin_enable() in
	// https://www.youtube.com/watch?v=36hk_Qov5Uo at 13:57
	// They are needed to effect GPIO oull-up/down changes
	// As per BCM2835 datasheet p101
	uintptr gppupd = MMIO_BASE + 0x200094; //see p100
	put32(gppupd, 0); // disable pull-up/down
	wait_cycles(150);
	uintptr gppudclk = MMIO_BASE + 0x200098 + bcm_pin/32;
	put32(gppudclk, 1 << (bcm_pin%32));
	// enable clock
	wait_cycles(150);
#if 0 // IFFY
	put32(gppupd, 0);
#endif
	put32(gppudclk, 0);
}

void gpio_clr(int bcm_pin)
{
        //volatile unsigned int* addr = (bcm_pin/32)*4 + MMIO_BASE + 0x200028;
        volatile uintptr addr = (bcm_pin/32)*4 + MMIO_BASE + 0x200028;
        put32(addr, 1 << (bcm_pin%32));
}

void gpio_set(int bcm_pin)
{
        volatile uintptr addr = (bcm_pin/32)*4 + MMIO_BASE + 0x20001C;
        put32(addr, 1 << (bcm_pin%32));
}

#if 0
/**
 * Make a mailbox call. Returns 0 on failure, non-zero on success
 */
int mbox_call(unsigned char ch, volatile unsigned int* mbox)
{
    unsigned int r = (((unsigned int)((unsigned long)&mbox)&~0xF) | (ch&0xF));
    /* wait until we can write to the mailbox */
    do{asm volatile("nop");}while(*MBOX_STATUS & MBOX_FULL);
    /* write the address of our message to the mailbox with channel identifier */
    *MBOX_WRITE = r;
    /* now wait for the response */
    while(1) {
        /* is there a response? */
        do{asm volatile("nop");}while(*MBOX_STATUS & MBOX_EMPTY);
        /* is it a response to our message? */
        if(r == *MBOX_READ)
            /* is it a valid successful response? */
            return mbox[1]==MBOX_RESPONSE;
    }
    return 0;
}
#endif
