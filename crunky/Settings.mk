RPI ?= 0
RPI ?= 3

ARMGNU = arm-none-eabi
#ARMGNU ?= arm-linux-gnueabihf
AS = $(ARMGNU)-as
CC = $(ARMGNU)-gcc
OBJDUMP = $(ARMGNU)-objdump
OBJCOPY = $(ARMGNU)-objcopy
LD = $(ARMGNU)-ld

ifeq ($(RPI),0)
	KERNEL = kernel
else
	KERNEL = kernel7
endif


IMG = $(KERNEL).img
ELF = $(KERNEL).elf
HEX = $(KERNEL).hex

COPS =  -nostdlib -nostartfiles -ffreestanding -I$(CRUNKY) 
COPS += -DRPI=$(RPI)

#AOPS = -DRPI=$(RPI)


