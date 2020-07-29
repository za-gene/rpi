
ARMGNU ?= arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror  -O3 -nostdlib -nostartfiles -ffreestanding 

IMG = kernel7.img
ELF = kernel7.elf
HEX = kernel.hex

$(IMG) : $(HEX)

OBJS = kernel.o vectors.o gpio.o delays.o mini_uart.o uart.o string.o mbox.o interrupts.o lfb.o

all : $(IMG)

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

%.o : %.c
	$(ARMGNU)-gcc $(COPS) -c $^ -o $@

#kernel.o : kernel.c
#	$(ARMGNU)-gcc $(COPS) -c $^ -o $@

$(ELF) : linker.ld $(OBJS)
	$(ARMGNU)-ld $(OBJS) -T linker.ld -o $@
	$(ARMGNU)-objdump -D $@ > kernel.list

$(IMG) : $(ELF)
	$(ARMGNU)-objcopy $(ELF) -O binary $@

$(HEX) : $(ELF)
	$(ARMGNU)-objcopy $^ -O ihex $(HEX)

