

"Master" templates:
* Makefile : 01-blink
* qlaunch : 04-timer-interrupts


* 01-led - turn on an LED. Pure ARM32 assembly. [source](From https://github.com/adamransom/bare_meta)
* 02-interrupts - ARM32/Pi3. Has interrupts, but you can't see anything working as such. Turns on a LED. [source](https://github.com/enricorov/Pinterrupt)
* 03-blink - ARM32/Pi3. Blink with polling delays. Nicer GPIO setup, too. Code re-org from 02.
* 04-timer-interrupts - ARM/Pi3. Blink LED each second using timer interrupts
* 05-uart0 - ARM/Pi3. Output to "regular" uart instead of uart1 (mini uart).
* 10-async - blinks 2 LEDs asynchronously using async library
