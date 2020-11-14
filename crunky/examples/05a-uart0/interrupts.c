#include "interrupts.h"


void do_nothing() {}

funcptr _IRQ_handler = do_nothing;

void set_irq_handler(funcptr fn)
{
	_IRQ_handler = fn;
}

void __attribute__ ((interrupt ("IRQ"))) IRQ_handler()
{
	_IRQ_handler();
}

