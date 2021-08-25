include Settings.mk

VPATH = ../pico # need for fat32

# a special case because it is the interrupt table
VEC_O = $(CRUNKY)/vectors.o

CR_OBJS = basal.o interrupts.o gpio.o \
	  mbox.o mini_uart.o \
	  uart0.o 

CR_OBJS += lfb.o font.psf.o
#CR_OBJS += vectors.o
CR_OBJS += nanolib-impl.o
CR_OBJS += bcm2835.o
#CR_OBJS += stdio.o
CR_OBJS += timers.o
#CR_OBJS += sd.o
CR_OBJS += tinyprintf.o
CR_OBJS += fat32.o
#CR_OBJS += sdcard-rpi.o
CR_OBJS += sd.o
CR_OBJS += crc8.o


# C-Lib
CR_OBJS += ctype.o snprintf.o stdlib.o string.o strtoul.o vsnprintf.o


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

