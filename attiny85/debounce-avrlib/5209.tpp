--title ATTINY85 2-BUTTON DEBOUNCE
--center Dec 2020

--newpage
--title OVERVIEW

* What
* Why
* How


--newpage
--title INTEGRATOR

Sample every 3ms (3*8=24ms)

Start:
    11111111

Shift inputs
    1111111_  < 0
    11111110  < 1
    11111101  < 0
    11111010  < 0
    11110100  etc.

Set output:
- Low when grate = 0x00
- High when = 0xFF


--newpage
--title LIMITATIONS

PB5 is reset button

Five buttons (maybe?)
- Tristate (i.e. floating input) or floating high


--newpage
--title SCHEMATIC
Pic ...
---
--exec feh tutorial.png &


--newpage
--title CODE
--color red
Unorthodox coding practises ahead
--color white
--beginoutput
int main()
{
        // PB:    43210 pin config (below)
        DDRB  = 0b00110; // set PB1,2 as outputs
---
        PORTB = 0b11110; // pullups & high
---
        u8 grate1 = 0xFF, grate2 = 0xFF;
---
loop:
        debounce(PB3, PB2, &grate2);
        debounce(PB4, PB1, &grate1);
        _delay_ms(3);
        goto loop;
--endoutput


--newpage
--title DEBOUNCE FUNCTIOM

--beginoutput
void debounce(u8 input, u8 output, u8* grate)
{
        *grate <<= 1; //shift the integrator
        *grate |= (bit_is_set(PINB, input) != 0);
        if(*grate == 0xFF) PORTB |= _BV(output);
        if(*grate == 0x00) PORTB &= ~_BV(output);
}
--endoutput


--newpage
--title COMPLETE CODE

main.cc (33 lines of code) ...


--newpage
--title DEMO

make
make flash
Simple lights
Proof of pudding with Arduino


--newpage
--title MODULE
Putting it all together.
Physical module
Schematic pic ...
---
--exec feh module.png &
