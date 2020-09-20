CC = sdcc
CFLAGS = -lstm8 -mstm8

stm8.lib : millis.rel
	sdcclib  $@ millis.rel

%.rel : %.c
	$(CC) $(CFLAGS) -c $<

clean :
	rm -f .ihx *.lst *.asm *.sym *.rst *.rel *.cdb *.ihx *.lk *.map *.mem *.lib
