CFLAGS = -lstm8 -mstm8

HEX = app.ihx

all : $(HEX)

interrupts.rel : interrupts.c
	sdcc $(CFLAGS) -c interrupts.c  

$(HEX) : main.c interrupts.rel
	sdcc main.c interrupts.rel -lstm8 -mstm8 --out-fmt-ihx -o$(HEX)
flash:
	stm8flash -cstlinkv2 -pstm8s103f3 -w$(HEX)

clean:
	rm -f .ihx *.lst *.asm *.sym *.rst *.rel
