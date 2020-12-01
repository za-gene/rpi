RPI ?= 3

ARMGNU = arm-none-eabi
#ARMGNU ?= arm-linux-gnueabihf
AS = $(ARMGNU)-as
CC = $(ARMGNU)-gcc
OBJDUMP = $(ARMGNU)-objdump
OBJCOPY = $(ARMGNU)-objcopy
LD = $(ARMGNU)-ld

KERNEL = kernel7

IMG = $(KERNEL).img
ELF = $(KERNEL).elf
HEX = $(KERNEL).hex

COPS =  -nostdlib -nostartfiles -ffreestanding -I$(CRUNKY) 
COPS += -DRPI=$(RPI)


# a special case because it is the interrupt table
VEC_O = $(CRUNKY)/vectors.o
