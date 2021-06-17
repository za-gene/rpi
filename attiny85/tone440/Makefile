VPATH = ..
CFLAGS = -I.. -g -Os -Wall -mcall-prologues -mmcu=attiny85

app.hex : app.out


.PHONY: flash clean

clean :
	rm -f *.hex *.obj *.o *.elf *.out

app.out : main.c timer1.c gpio.c
	avr-gcc $(CFLAGS) $^ -o app.out
	avr-objcopy -O ihex app.out app.hex

flash : app.hex
	install-hex app.hex

