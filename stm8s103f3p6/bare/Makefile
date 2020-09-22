CC = sdcc
CFLAGS = -lstm8 -mstm8 -I .

RELS = millis.rel gpio.rel

stm8.lib : $(RELS)
	#sdcclib  $@ millis.rel
	sdar -rc stm8.lib $^

%.rel : %.c
	$(CC) $(CFLAGS) -c $<

clean :
	rm -f .ihx *.lst *.asm *.sym *.rst *.rel *.cdb *.ihx *.lk *.map *.mem *.lib
