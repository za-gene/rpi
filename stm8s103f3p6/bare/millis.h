#pragma once

void init_millis();
uint32_t millis();
void timer4_millis_isr() __interrupt(TIM4_ISR); // need to include this here, even though not called

