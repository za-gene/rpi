# 05-uart0

Outputs to uart0 at 115200 baud. Confirmed working on a Pi3 although qemu still doesn't seem to like it. 

* kernel_5 uart0 test. UART0 is the "main" uart
* kernel_6 framebuffer test. Displays pic of Homer Simpson
* kernel framebuffer font test


## SD CARD

Yourr SD will need to contain bootcode.bin, fixup.dat, start.elf, and of course the generated kernel.img. Exclude config.txt

## REFERENCES


* [bztsrc](https://github.com/bztsrc/raspi3-tutorial.git)  does most of the heavy lifting for this project. Fonts were also obtained from that repo.
