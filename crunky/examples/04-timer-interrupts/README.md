# 04-timer-interrupts

This project implements a timer interrupt. It toggles pin GPIO BCM19 (physical pin 35) every second and outputs a "." on mini uart (UART1). The serial port is on BCM14 (TX) and BMC15 (RX). 

This can probably be adapted for other interrupts. "Somehow".

The key files to focus on are kernel.c and vectors.s. The other files are just support files. 

The important bits in kernel.c is the function IRQ_handler(), which handles the timer. kernel_main() sets up the timer and enables it.

vectors.s does some behind-the-scenes stuff. It copies the interrupt vector to 0x0000. It has a pointer to location irq_handler, which gets called (actually irq) when there is an interrupt. ASM routine irq saves the stack, calls the IRQ_handler() routine which kernel.c supplies, then clears the timer interrupt and restores the stack.

If you decide against defining an irq handler you could always comment out the call to it in the asm file.


## SD CARD

Yourr SD will need to contain bootcode.bin, fixup.dat, start.elf, and of course the generated kernel.img. Exclude config.txt

## REFERENCES

* [BrianSideBotham](https://www.valvers.com/open-software/raspberry-pi/bare-metal-programming-in-c-part-4/) interrupt discussion
* [jsandler](https://jsandler18.github.io/tutorial/interrupts.html)'s article on interrupts and setting up the exception vector table. It talks about the need for special prologues and epilogues for interrupt handlers. My approach does not need this because the name of the function called is hard-coded and wrapped inside the necessary special handling by the ASM which calls it.
* [Setting up exceptions made easy](https://www.raspberrypi.org/forums/viewtopic.php?f=72&t=281156) - where I discuss some of the rationales behind setting up interrupts

