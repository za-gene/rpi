# GPIO - low-level notes

## Registers

You can read and write to all these registers.


**DDRB** - Port B Data Direction Register

Initial value 0. 0 for input. 1 for output.


**PORTB** - Port B Data Register

Initial value 0. If a pin is configured for input (see DDRB), then setting the bit to 1 will make it a pull-up input. If a pin is configured for output, then setting the bit to 1 will output high; low otherwise.


**PINB** Port B Input Pins Address

Indeterminate intial value. Setting a bit to 1 toggles the corresponding bit in PORTB. The SBI
instruction can be used to toggle one single bit in a port.
