# gpio

Folder [01-blink](01-blink) gives an example of how to blink some LEDs using GPIO.

## Configuring the pins

The pins at your disposal belong to a number of "ports". PC13, for example, belongs to port C. At a high level, to configure a pin, you:
* enable its port via RCC_APB2ENR
* Set one of the CRL or CRH register for that port to either input or output.

RCC is the Reset and Clock Control. RCC_APB2ENR is the APB2 peripheral clock enable register.
This register can be used to enable the clock for ADC (Analog-to-Digital Conversion),, USART, SPI, some timers, alternate functions, and most importantly for out purposes: the GPIO 
port. 
To enable port C, you would issue a command like:
```
RCC_APB2ENR |= IOPCEN;
```
To enable port B, you would use IOPBEN instead of IOPCEN, and so on. 
RCC_APB2ENR is described in section 7.3.7 of the datasheet.

GPIOx_CRL (section 9.2.1) is the port Configuration Register Low for a port, 
and GPIOx_CRH (section 9.2.2) is the high.
Enabling inputs and outputs for a pin takes 4 registers, requiring 2 32-bit registers to
configure all the pins on a port. Hence the Low and the High. The Low register is
for pins 0..7 of the port in question, and the high register is for pins 8..15.

For PC13, for example, you need to use the high register. 8 pins per register
times 4 configuration bits per pin makes 32 bits. The datasheets tells you which bits
cover which pins. 

The 4 bits comprise 2 bits for the MODE, and 2 bits for CNF (configuration). 
If MODE is 0b00, that means the pin is configured for input. Setting mode to
0b00 because analog input, 0b01 is floating input (probably what you would normally use),
and 0b10 is for pull-up (which you'd use with a button).

If you set MODE to 0b10, then the pin is configured to output with a max speed of 2MHz.
It can also be configured for 10MHz ans 5MHz. I shall assume 2MHz is the most typical. Then,
CNF can be either configures with the pin's alternate function, an open drain,
or push-pull. Push-pull is the standard one you'll want to use.

*Typically*, the combination of CNF and MODE will be one of:
```
0b0000 - analog input
0b0100 - floating input
0b1000 - pull-up input
0b0010 - output
```

To set the CNF and MODE, you would first mask out 4 bits in the relevant configuration
register, and then mask them back in.

Let's say that you want to see PC13 for output. The commands you would issue are:
```
RCC_APB2ENR |= RCC_IOPCEN; // enable port C
GPIOC->CRH   &= 0xFF0FFFFF; // mask out the CNF/MODE bits for the pin
GPIOC->CRH   |= 0x00200000; // fill the CNF/MODE bits with the ones you require
```

Function `gpio_mode_out()` looks complicated, but it's just computationally figures
out what the register addresses are, whether is needs the Low or High register,
the relevant bit positions, and so on.

## Setting the pin state

After a pin has been set for output, for example, you will then want to
set the pin either low or high. You can find out if the pin is high or low
from the Output Data Register (GPIOx_ODR) in section 9.2.4.

The ODR register is writable, so you can set a pin high or low by writing to the
register. But I advise against it. Reading and then writing is not an atomic
operation, so it is possible that you might be clobbering changes made during an interrupt.

Instead, use the GPIOx_BSRR (Bit Set/Reset Register), as described in section 9.2.5. 
This enables you to set a pin high using bits 15..0, or set then low using bits
31..16. This is how `gpio_write()` does it.

It is also possible to use the GPIOx_BRR (Bit Reset Register) to set pin(s) low;
but this seems redundant, as GPIO_BSRR already offers this functionality.

## Built-in LED

The built-in LED for the blue pill is PC13. gpio.h also provides the alias BUILTIN_LED.
To enable it, you will want to issue the command `gpio_mode_out(BUILTIN_LED)`. 
The pin itself acts as a normal GPIO pin, but counterintuitively,
to light up the builtin LED you need to write the pin low (`gpio_write(0)`),
and to turn it off, use `gpio_write(1)`. 
need to set the 

