CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
BIN = arm-none-eabi-objcopy
STL = st-flash
CFLAGS = -mthumb -mcpu=cortex-m3 -ggdb -O0 
CFLAGS += -nostdlib
CFLAGS += -I..
#CFLAGS = -mthumb -mcpu=cortex-m3 -ggdb -O0 -nostdlib --specs=nano.specs

all: app.bin 

crt1.o: crt1.s
	$(AS) -o crt1.o crt1.s

main.o: main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

app.elf: linker.ld crt1.o main.o ../libblue.a
	#$(LD) -T linker.ld  -o app.elf main.o -L/usr/lib/arm-none-eabi/newlib/thumb  -lc_nano
	#$(LD) -T linker.ld -o app.elf crt1.o main.o -L .. -lblue
	$(LD) -T linker.ld -o app.elf main.o crt1.o -L .. -lblue
	#$(LD) -T linker.ld -o app.elf crt.o main.o -lc

app.bin: app.elf
	$(BIN) -O binary app.elf app.bin
	arm-none-eabi-objdump -d app.elf >app.txt

clean:
	rm -f *.o *.elf *.bin

.flash: app.bin
	touch .flash

flash : .flash
	$(STL) write app.bin 0x8000000

erase:
	$(STL) erase

dump:
	arm-none-eabi-objdump -d app.elf
