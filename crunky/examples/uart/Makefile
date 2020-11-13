ARMGNU ?= aarch64-linux-gnu

COPS = -Wall -nostdlib -nostartfiles -ffreestanding -Iinclude -mgeneral-regs-only
INC = -I../../include

SRC_DIR = ../../src
LIB_DIR = ../../src
BUILD_DIR = ../../build


all : kernel8.img

clean:
	rm -f *d *o *elf *img

OBJS = delays.o gpio.o utils.o mini_uart.o boot.o mm.o \
       kernel.o

kernel8.img: $(LIB_DIR)/linker.ld $(OBJS)
	$(ARMGNU)-ld -T $(SRC_DIR)/linker.ld -o kernel8.elf  $(OBJS) # $(OBJ_FILES)
	$(ARMGNU)-objcopy kernel8.elf -O binary kernel8.img


mini_uart.o : $(SRC_DIR)/mini_uart.c
	$(ARMGNU)-gcc $(INC) $(COPS) -MMD -c $< -o $@

boot.o : $(SRC_DIR)/boot.S
	$(ARMGNU)-gcc $(INC) -MMD -c $< -o $@

mm.o : $(SRC_DIR)/mm.S
	$(ARMGNU)-gcc $(INC) -MMD -c $< -o $@

utils.o : $(SRC_DIR)/utils.S
	$(ARMGNU)-gcc $(INC) -MMD -c $< -o $@

gpio.o : $(SRC_DIR)/gpio.c
	$(ARMGNU)-gcc $(INC) $(COPS) -MMD -c $< -o $@

delays.o : $(SRC_DIR)/delays.c
	$(ARMGNU)-gcc $(INC) $(COPS) -MMD -c $< -o $@

kernel.o : kernel.c
	$(ARMGNU)-gcc $(INC) $(COPS) -MMD -c $< -o $@
