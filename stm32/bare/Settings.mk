ARMGNU = arm-none-eabi
AS = $(ARMGNU)-as
CC = $(ARMGNU)-gcc
CXX = $(ARMGNU)-g++
OBJDUMP = $(ARMGNU)-objdump
OBJCOPY = $(ARMGNU)-objcopy
LD = $(ARMGNU)-ld

BIN = arm-none-eabi-objcopy
STL = st-flash

CFLAGS = -mthumb -mcpu=cortex-m3 
CFLAGS += -nostdlib
CFLAGS += -I${STM32}
LDFLAGS = -L${STM32} -T linker.ld
LDFLAGS += -Os --gc-sections # remove unneeded code. Might cause problems, though
CXXFLAGS = $(CFLAGS)
CXXFLAGS += -fno-exceptions

KERNEL = app
IMG = $(KERNEL).img
ELF = $(KERNEL).elf
HEX = $(KERNEL).hex

