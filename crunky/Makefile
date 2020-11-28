include Settings.mk
#ARMGNU ?= arm-none-eabi
#ARMGNU = arm-linux-gnueabihf
#ARMGNU = aarch64-linux-gnu
COPS = -Wall -nostdlib -nostartfiles -ffreestanding -I. 
#ASMOPS = -Iinclude 

#BUILD_DIR = build
#SRC_DIR = src

CR_OBJS = basal.o delays.o interrupts.o gpio.o lfb.o font.psf.o font.sfn.o \
	  mbox.o memory.o mini_uart.o \
	  stdio.o string.o uart0.o

CR_LIB = libcrunky.a


all : $(CR_OBJS) $(CR_LIB) $(VEC_O)


$(CR_LIB) : $(CR_OBJS)
	ar -crs $@ $^

$(VEC_O) : $(CRUNKY)/vectors.s
	$(AS) $< -o $@

%.o : %.c
	$(ARMGNU)-gcc $(COPS) -c $^ -o $@

clean :
	rm -rf *.img *.o *.a

