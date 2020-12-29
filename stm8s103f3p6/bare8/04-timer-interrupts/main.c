#include "../stm8.h"



#define LED (1<<5)

// Default system clock will be 2MHz.
// We set 128 as prescaler, then each tick of timer 2 will be in 64 micro seconds.
// So, timer will generate overflow interrupt in each second,
// when counter reaches at 15625 (1S/64uS) which is set in auto reload register.
// There are two 8 bit regisgers to hold 16 bit value for ARR. So, we create a 16
// bit unsigned number.
const uint16_t reload_value = 15625;


void timer_isr() __interrupt(TIM2_OVF_ISR) {
    PORTB->ODR ^= LED; // Toggle PB5 output
    TIM2_SR1 &= ~TIM2_SR1_UIF; // Clear interrupt flag
}

void main() {
    enable_interrupts();

    PORTB->DDR |= LED; // 0x00001000 PB5 is now output
    PORTB->CR1 |= LED; // 0x00001000 PB5 is now pushpull

    TIM2_PSCR = 0b00000111; //  Prescaler = 128
    // Fill auto reload registers.
    // We need to put MSB and LSB in separate 8 bit registers.
    // Also, as per datasheet, we have to put value in ARRH first, then in ARRL.
    TIM2_ARRH = reload_value >> 8;
    TIM2_ARRL = reload_value & 0x00FF;

    TIM2_IER |= TIM2_IER_UIE; // Enable Update Interrupt
    TIM2_CR1 |= TIM2_CR1_CEN; // Enable TIM2

    while (1) {
        // do nothing
    }
}

