# 05-uart0

Outputs to uart0 at 115200 baud. Confirmed working on a Pi3 although qemu still doesn't seem to like it. 

## SD CARD

Yourr SD will need to contain bootcode.bin, fixup.dat, start.elf, and of course the generated kernel.img. Exclude config.txt

## REFERENCES


* [bztsrc](https://github.com/bztsrc/raspi3-tutorial/tree/master/05_uart0) partial source for this project
