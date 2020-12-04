include Settings.mk

#ARMGNU ?= arm-none-eabi
#ARMGNU = arm-linux-gnueabihf
#ARMGNU = aarch64-linux-gnu
#ASMOPS = -Iinclude 

#BUILD_DIR = build
#SRC_DIR = src

# a special case because it is the interrupt table
VEC_O = $(CRUNKY)/vectors.o

CR_OBJS = basal.o interrupts.o gpio.o lfb.o font.psf.o font.sfn.o \
	  mbox.o memory.o mini_uart.o \
	  sd.o stdio.o string.o uart0.o \
	  timers.o

#CR_OBJS += printf.o
#CR_OBJS += snprintf.o
CR_OBJS += tinyprintf.o


CR_LIB = libcrunky.a


all : $(CR_OBJS) $(CR_LIB) vectors.o


$(CR_LIB) : $(CR_OBJS)
	ar -crs $@ $^

#$(VEC_O) : $(CRUNKY)/vectors.S
vectors.o : vectors.S	
	#$(AS)   $< -o $@
	#$(AS) -DRPI=$(RPI)  -c $< -o $@
	$(CC) $(COPS)  -c $< -o $@

%.o : %.c
	$(CC) $(COPS) -c $^ -o $@

clean :
	rm -rf *.img *.o *.a

