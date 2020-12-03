// released into the public domain

#include <basal.h>
#include <gpio.h>
#include <timers.h>



void gpio_sel(int bcm_pin, int mode)
{
        u32 gpfsel = (bcm_pin/10)*4+ PBASE+ 0x200000;
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
	u32 gppupd = PBASE + 0x200094; //see p100
	put32(gppupd, 0); // disable pull-up/down
	wait_cycles(150);
	u32 gppudclk = PBASE + 0x200098 + bcm_pin/32;
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
        u32 addr = (bcm_pin/32)*4 + PBASE + 0x200028;
        put32(addr, 1 << (bcm_pin%32));
}

void gpio_set(int bcm_pin)
{
        u32 addr = (bcm_pin/32)*4 + PBASE + 0x20001C;
        put32(addr, 1 << (bcm_pin%32));
}

int gpio_get(int bcm_pin)
{
	u32 addr = (bcm_pin/32)*4 + PBASE + 0x200034;
	u32 bits = get32(addr);
	return 1 & (bits >> (bcm_pin%32));
}

void gpio_toggle(int bcm_pin)
{
	if(gpio_get(bcm_pin))
		gpio_clr(bcm_pin);
	else
		gpio_set(bcm_pin);
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
