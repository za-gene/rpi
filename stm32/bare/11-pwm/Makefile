CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
BIN = arm-none-eabi-objcopy
STL = st-flash
CFLAGS = -mthumb -mcpu=cortex-m3 
#CFLAGS += -ggdb -O0 
CFLAGS += -nostdlib
CFLAGS += -I${STM32}
#CFLAGS = -mthumb -mcpu=cortex-m3 -ggdb -O0 -nostdlib --specs=nano.specs
LDFLAGS = -L${STM32} -T linker.ld
#LDFLAGS += -lgcc # need for stuff like integer division
LDFLAGS += -Os --gc-sections # remove unneeded code. Might cause problems, though

all: app.bin 

main.o: main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

app.elf: main.o 
	$(LD) $(LDFLAGS) -o app.elf main.o  -lblue
	#$(LD) $(LDFLAGS) -o app.elf main.o  --gc-sections ${STM32}/*.o

app.bin: app.elf
	$(BIN) -O binary app.elf app.bin
	arm-none-eabi-objdump -d app.elf >app.txt

clean:
	rm -f *.o *.elf *.bin app.txt .flash

.flash: app.bin
	touch .flash

flash : .flash
	$(STL) write app.bin 0x8000000

erase:
	$(STL) erase

dump:
	arm-none-eabi-objdump -d app.elf
