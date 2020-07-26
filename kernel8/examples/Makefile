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



C_FILES = $(wildcard $(LIB_DIR)/*.c)
ASM_FILES = $(wildcard $(LIB_DIR)/*.S)
OBJ_FILES = $(C_FILES:$(LIB_DIR)/%.c=$(BUILD_DIR)/%_c.o)
OBJ_FILES += $(ASM_FILES:$(LIB_DIR)/%.S=$(BUILD_DIR)/%_s.o)


DEP_FILES = $(OBJ_FILES:%.o=%.d)
-include $(DEP_FILES)



OBJS = blink.o delays.o gpio.o utils.o

kernel8.img: $(LIB_DIR)/linker.ld $(OBJS)
	$(ARMGNU)-ld -T $(SRC_DIR)/linker.ld -o kernel8.elf  $(OBJS) # $(OBJ_FILES)
	$(ARMGNU)-objcopy kernel8.elf -O binary kernel8.img


utils.o : ../src/utils.S
	$(ARMGNU)-gcc -I../include $(COPS) -MMD -c $< -o $@

gpio.o : ../src/gpio.c
	$(ARMGNU)-gcc -I../include $(COPS) -MMD -c $< -o $@

delays.o : ../src/delays.c
	$(ARMGNU)-gcc -I../include $(COPS) -MMD -c $< -o $@

blink.o : blink.c
	$(ARMGNU)-gcc -I../include $(COPS) -MMD -c $< -o $@
