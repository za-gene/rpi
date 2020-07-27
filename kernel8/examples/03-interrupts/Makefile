
ARMGNU ?= arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 

IMG = kernel7.img
ELF = kernel7.elf
HEX = kernel.hex

gcc : $(HEX) $(IMG)

all : gcc clang

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.clang.opt.s

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

kernel.o : kernel.c
	$(ARMGNU)-gcc $(COPS) -c $^ -o $@

$(ELF) : linker.ld vectors.o kernel.o 
	$(ARMGNU)-ld vectors.o kernel.o -T linker.ld -o $@
	$(ARMGNU)-objdump -D $@ > kernel.list

$(IMG) : $(ELF)
	$(ARMGNU)-objcopy $(ELF) -O binary $@

$(HEX) : $(ELF)
	$(ARMGNU)-objcopy $^ -O ihex $(HEX)

