ARMGNU ?= arm-none-eabi
#ARMGNU = arm-linux-gnueabihf
#ARMGNU = aarch64-linux-gnu
COPS = -Wall -nostdlib -nostartfiles -ffreestanding 
#ASMOPS = -Iinclude 

#BUILD_DIR = build
#SRC_DIR = src

OBJS = delays.o interrupts.o vectors.o lfb.o font.psf.o font.sfn.o mbox.o memory.o
LIB = libcrunky.a

all : $(OBJS) $(LIB)


$(LIB) : $(OBJS)
	ar -crs $@ $^

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

%.o : %.c
	$(ARMGNU)-gcc $(COPS) -c $^ -o $@

clean :
	rm -rf $(BUILD_DIR) *.img *.o $(LIB)

