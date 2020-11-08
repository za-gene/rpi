ARMGNU ?= arm-none-eabi

COPS = -Wall -nostdlib -nostartfiles -ffreestanding 
#ASMOPS = -Iinclude 

#BUILD_DIR = build
#SRC_DIR = src

OBJS = interrupts.o vectors.o lfb.o font.psf.o font.sfn.o mbox.o

all : $(OBJS)



vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

%.o : %.c
	$(ARMGNU)-gcc $(COPS) -c $^ -o $@

clean :
	rm -rf $(BUILD_DIR) *.img *.o

