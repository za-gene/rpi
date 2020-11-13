#pragma once

typedef void (*funcptr)();

void set_irq_handler(funcptr fn);
