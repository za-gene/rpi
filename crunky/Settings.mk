RPI ?= 3

ARMGNU = arm-none-eabi
#ARMGNU ?= arm-linux-gnueabihf
AS = $(ARMGNU)-as
CC = $(ARMGNU)-gcc


IMG = kernel7.img
ELF = kernel7.elf
HEX = kernel.hex

COPS =  -nostdlib -nostartfiles -ffreestanding -I$(CRUNKY) -DRPI=$(RPI)


# a special case because it is the interrupt table
VEC_O = $(CRUNKY)/vectors.o
