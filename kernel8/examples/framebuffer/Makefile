ARMGNU ?= arm-none-eabi

COPS = -Wall -nostdlib -nostartfiles -ffreestanding -Iinclude
INC = -I../../include

#SRC_DIR = ../../src
SRC_DIR = .
LIB_DIR = ../../src
BUILD_DIR = ../../build

KERN = kernel7

all : $(KERN).img

clean:
	rm -f *.d *.o *.elf *.img *.bin *.bc *.list

#OBJS = delays.o gpio.o utils.o mini_uart.o  mm.o \
#       kernel.o

OBJS = kernel.o

%.o : %.c
	$(ARMGNU)-gcc $(COPS) -c $^ -o $@

$(KERN).img: $(LIB_DIR)/linker.ld $(OBJS)
	$(ARMGNU)-ld -T $(SRC_DIR)/linker.ld -o $(KERN).elf  $(OBJS) # $(OBJ_FILES)
	$(ARMGNU)-objcopy $(KERN).elf -O binary $(KERN).img

