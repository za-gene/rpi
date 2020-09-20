CC = sdcc
CFLAGS = -lstm8 -mstm8

HEX = app.ihx

RELS = main.rel 

all : $(HEX)


%.rel : %.c
	$(CC) $(CFLAGS) -c $<

$(HEX) : $(RELS)
	$(CC) $(CFLAGS) $(RELS) stm8.lib -L .. --out-fmt-ihx -o$(HEX)
	#$(CC) $(CFLAGS) $(RELS)  --out-fmt-ihx -o$(HEX)

flash: $(HEX)
	stm8flash -cstlinkv2 -pstm8s103f3 -w$(HEX)

clean:
	rm -f .ihx *.lst *.asm *.sym *.rst *.rel *.cdb *.ihx *.lk *.map *.mem
