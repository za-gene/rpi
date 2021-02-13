include Settings.mk


# a special case because it is the interrupt table
VEC_O = $(CRUNKY)/vectors.o

CR_OBJS = basal.o interrupts.o gpio.o lfb.o font.psf.o font.sfn.o \
	  mbox.o memory.o mini_uart.o \
	  nanolib-impl.o \
	  sd.o uart0.o \
	  timers.o

CR_OBJS += vectors.o
CR_OBJS += bcm2835.o
CR_OBJS += fat.o


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

