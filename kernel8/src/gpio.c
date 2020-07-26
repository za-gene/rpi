// released into the public domain

#include <gpio.h>
#include <utils.h>

typedef unsigned int u32; // irrespective of architecture
typedef  unsigned long uintptr; // architecture-dependent

void gpio_sel(int bcm_pin, int mode)
{
        uintptr gpfsel = (bcm_pin/10)*4+ MMIO_BASE+ 0x200000;
        u32 value = get32(gpfsel);
        u32 shift = (bcm_pin%10)*3;
        value &= ~(0b111 << shift); // clear the bits
        value |= (mode << shift); // set to desired mode
        put32(gpfsel, value);

}

void gpio_clr(int bcm_pin)
{
        //volatile unsigned int* addr = (bcm_pin/32)*4 + MMIO_BASE + 0x200028;
        uintptr addr = (bcm_pin/32)*4 + MMIO_BASE + 0x200028;
        put32(addr, 1 << (bcm_pin%32));
}

void gpio_set(int bcm_pin)
{
        uintptr addr = (bcm_pin/32)*4 + MMIO_BASE + 0x20001C;
        put32(addr, 1 << (bcm_pin%32));
}


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
