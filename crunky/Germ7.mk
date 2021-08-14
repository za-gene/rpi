include $(CRUNKY)/Settings.mk

VPATH=$(CRUNKY)

AOPS = --warn --fatal-warnings 
#COPS =  -nostdlib -nostartfiles -ffreestanding -I$(CRUNKY)

#CR_OBJS = basal.o interrupts.o gpio.o lfb.o font.psf.o font.sfn.o \	  
#          mbox.o memory.o mini_uart.o \
#          nanolib-impl.o \
#          sd.o uart0.o \
#          timers.o 

#CR_OBJS += vectors.o
#CR_OBJS += bcm2835.o
#CR_OBJS += fat.o
#CR_OBJS += tat.o



MNTDIR = $(shell ls /media/$(USER) | head -1)



$(IMG) : $(HEX)

LIB_CRUNKY = libcrunky.o

$(LIB_CRUNKY) : $(CRUNKY_OBJS)

all : $(IMG)

clean :
	rm -f *.o *.bin *.hex *.elf *.dis *.img *.a $(BUILT)


%.o : %.c
	$(CC) $(COPS) -c $^ -o $@

%.o : %.cc
	$(CXX) $(CXXFLAGS) -c $^ -o $@

LINKER = $(CRUNKY)/linker.ld


#NEWLIB = -L/usr/lib/arm-none-eabi/newlib/hard
#NEWLIB += -lstdc++_nano  -lsupc++_nano
#NEWLIB += -lc_nano
#NEWLIB += -lg_nano
#GCCLIB = -L/usr/lib/gcc/arm-none-eabi/7.3.1/hard -lgcc

#LD_CRUNKY = -L$(CRUNKY) -lcrunky -m
LD_CRUNKY = -L$(CRUNKY) -lcrunky

#LIBS =  $(NEWLIB) $(GCCLIB) -lm

$(ELF) : $(LINKER) $(CR_OBJS) $(OBJS)  $(LIBUSPI) $(LIB_CRUNKY)
	#$(LD)    $(CR_OBJS) $(OBJS) -T $(LINKER)  $(LIBS) -o $@
	$(LD)    $(OBJS) $(CRUNKY)/vectors.o -T $(LINKER)  $(LIBS) $(LD_CRUNKY) -o $@
	$(OBJDUMP) -D $@ > $(KERNEL).dis

$(IMG) : $(ELF)
	$(OBJCOPY) $(ELF) -O binary $@

$(HEX) : $(ELF)
	$(OBJCOPY) $^ -O ihex $(HEX)

flash : install

install:
	#echo $(MNTDIR)
	#cp $(IMG) /media/pi/50BD-6FEC
	cp $(IMG) /media/pi/$(MNTDIR)
