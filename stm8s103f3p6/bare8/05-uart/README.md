# stm8 uart

Prints out a message, and echos user input at 115200 baud

## BRR: Baud Rate Register calculations

You set the baud rate of uart bia UART1_BRR1 (section 22.7.3. page 364, of the 
stm8 datasheet) and UART_BRR2 (section 22.7.4).

The stm8 seems to have a bizarre way of setting up the baud. Calculate UART_DIV, the uart divisor as a uint32. This is the clock speed divided by the baud you want to transmit/receive at. Set UART1_BRR2 first, then UART1_BRR1 last. UART1_BRR2 bits [7:4] should have UART_DIV bits [15:12], and UART1_BRR2 bits [3:0] should have UART_DIV bits [3:0]. UART_BRR1 should have UART_DIV bits [11:4].

Huh? In simple(r) terms, if you calculate UART_DIV to be of the hex form 0xPQRS, then set UART1_BRR2 to 0xPS, and UART_BRR1 to 0xQR.

Remember: the default clock speed is 2MHz, but it can be set to 16MHz if you need to.

### Example: Determine UART_BRR2 and UART_BRR1 for 16MHz clock, 115200 baud

Compute UART_DIV as follows:
```
UART_DIV = 16,000,000 / 115,200 = 138.889, call it 139 = 0x008B
```

Thus P=0, Q=0, R=8, S=B, so UART1_BRR2= 0x0B, UART1_BRR=0x08

### Example: Determine UART_BRR2 and UART_BRR1 for 2MHz clock, 115200 baud

This is used in the example code.

```
UART_DIV = 2,000,000/115,200 = 17.36, call it 17 = 0x0011
```

Therefore UART_BRR2 = 0x01, and likewise UART_BRR1 = 0x01.


## Connecting

**DANGER:** I doubt if the stm8's pins are 5V tolerant, so a regular USB-to-Serial adapter might fry the board's pins. Work with 3V supplies to ensure you don't damage your mcu.

Pin PD5 is UART1_TX, so it must be connected to the RX pin of your serial device. PD6 is UART1_RX, and must be connected to the TX pin of your serial device.



## See also

* [alternative](http://www.colecovision.eu/stm8/STM8S003F3%20Dev%20Board%20Power%20Input%20Onboard%203V3%20Regulator.shtml) implementation
* [source](https://blog.mark-stevens.co.uk/2012/08/using-the-uart-on-the-stm8s-2/)
