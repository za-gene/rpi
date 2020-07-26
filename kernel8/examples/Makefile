ARMGNU ?= aarch64-linux-gnu

COPS = -Wall -nostdlib -nostartfiles -ffreestanding -Iinclude -mgeneral-regs-only
ASMOPS = -Iinclude

SRC_DIR = ../src
LIB_DIR = ../src
BUILD_DIR = ../build

#$(BUILD_DIR)/%_c.o: $(SRC_DIR)/%.c
#	mkdir -p $(@D)
#	$(ARMGNU)-gcc $(COPS) -MMD -c $< -o $@

#$(BUILD_DIR)/%_s.o: $(SRC_DIR)/%.S
#	$(ARMGNU)-gcc $(ASMOPS) -MMD -c $< -o $@



#C_FILES = $(wildcard $(LIB_DIR)/*.c)
#ASM_FILES = $(wildcard $(LIB_DIR)/*.S)
#OBJ_FILES = $(C_FILES:$(LIB_DIR)/%.c=$(BUILD_DIR)/%_c.o)
#OBJ_FILES += $(ASM_FILES:$(LIB_DIR)/%.S=$(BUILD_DIR)/%_s.o)


#DEP_FILES = $(OBJ_FILES:%.o=%.d)
#-include $(DEP_FILES)


all : kernel8.img

clean:
	rm -f *d *o *elf *img

OBJS = blink.o delays.o gpio.o utils.o mini_uart.o boot.o mm.o

kernel8.img: $(LIB_DIR)/linker.ld $(OBJS)
	$(ARMGNU)-ld -T $(SRC_DIR)/linker.ld -o kernel8.elf  $(OBJS) # $(OBJ_FILES)
	$(ARMGNU)-objcopy kernel8.elf -O binary kernel8.img


mini_uart.o : ../src/mini_uart.c
	$(ARMGNU)-gcc -I../include $(COPS) -MMD -c $< -o $@

boot.o : ../src/boot.S
	$(ARMGNU)-gcc -I../include -MMD -c $< -o $@

mm.o : ../src/mm.S
	$(ARMGNU)-gcc -I../include -MMD -c $< -o $@

utils.o : ../src/utils.S
	$(ARMGNU)-gcc -I../include -MMD -c $< -o $@

gpio.o : ../src/gpio.c
	$(ARMGNU)-gcc -I../include $(COPS) -MMD -c $< -o $@

delays.o : ../src/delays.c
	$(ARMGNU)-gcc -I../include $(COPS) -MMD -c $< -o $@

blink.o : blink.c
	$(ARMGNU)-gcc -I../include $(COPS) -MMD -c $< -o $@
