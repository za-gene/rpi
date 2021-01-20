#pragma once

#include <basal.h>

typedef void (*funcptr)();

void set_irq_handler(funcptr fn);
void enable_irq ();
void disable_irq ();
void enable_arm_timer_irq();

// Per BCM2837 s7.5 p112 for Pi3
#define IBASE (PBASE + 0xB000) 

#define IRQ_BASIC 		REG(IBASE + 0x200) // IRQ basic pending
#define IRQ_PEND1 		REG(IBASE + 0x204) // IRQ pending 1
#define IRQ_PEND2 		REG(IBASE + 0x208) // IRQ pending 2
#define IRQ_FIQ_CONTROL		REG(IBASE + 0x20C) // FIQ control
#define IRQ_ENABLE_1		REG(IBASE + 0x210) // Enable IRQs 1
#define IRQ_ENABLE_2		REG(IBASE + 0x214) // Enable IRQs 2
#define IRQ_ENABLE_BASIC	REG(IBASE + 0x218) // Enable Basic IRQs
#define IRQ_DISABLE_1		REG(IBASE + 0x21C) // Disable IRQs 1
#define IRQ_DISABLE_2		REG(IBASE + 0x220) // Disable IRQs 2
#define IRQ_DISABLE_BASIC 	REG(IBASE + 0x224) // Disable Basic IRQs

