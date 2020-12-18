CC = arm-none-eabi-gcc
CXX = arm-none-eabi-g++
LD = arm-none-eabi-ld
BIN = arm-none-eabi-objcopy
STL = st-flash
CFLAGS = -mthumb -mcpu=cortex-m3 
#CFLAGS += -ggdb -O0 
CFLAGS += -nostdlib
#CFLAGS += -nodefaultlib
CFLAGS += -I${STM32}
LDFLAGS = -L${STM32} -T linker.ld
#LDFLAGS += -lgcc # need for stuff like integer division
LDFLAGS += -Os --gc-sections # remove unneeded code. Might cause problems, though
CXXFLAGS = $(CFLAGS)
CXXFLAGS += -fno-exceptions

all: app.bin 

main.o: main.cc
	$(CXX) $(CXXFLAGS) -c -o main.o main.cc

app.elf: main.o 
	$(LD) $(LDFLAGS) -o app.elf main.o  -lblue -L /usr/lib/arm-none-eabi/newlib -lstdc++_nano
	#$(LD) $(LDFLAGS) -o app.elf main.o  -lblue

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
