HEX = app.ihx

$(HEX) : main.c
	sdcc main.c -lstm8 -mstm8 --out-fmt-ihx -o$(HEX)
flash:
	stm8flash -cstlinkv2 -pstm8s103f3 -w$(HEX)

clean:
	rm app.*
