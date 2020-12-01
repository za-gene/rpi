include $(CRUNKY)/Settings.mk


AOPS = --warn --fatal-warnings 
#COPS =  -nostdlib -nostartfiles -ffreestanding -I$(CRUNKY)



#BUILT = font.psf.h font.sfn.h
$(IMG) : $(HEX)

#CRUNKY_OBJS = vectors.o interrupts.o lfb.o  font.psf.o font.sfn.o \
#	memory.o mbox.o  gpio.o delays.o uart.o string.o # mini_uart.o

LIB_CRUNKY = libcrunky.o

$(LIB_CRUNKY) : $(CRUNKY_OBJS)

#FONTSO = font.psf.o font.sfn.o

all : $(IMG)

clean :
	rm -f *.o *.bin *.hex *.elf *.list *.img *.a $(BUILT)


%.o : %.c
	$(CC) $(COPS) -c $^ -o $@


LINKER = $(CRUNKY)/linker.ld


$(ELF) : $(LINKER) $(OBJS)  $(LIBUSPI) $(VEC_O)
	$(LD) $(VEC_O)   $(OBJS) -T $(LINKER)  -L$(CRUNKY) -lcrunky -o $@
	$(OBJDUMP) -D $@ > $(KERNEL).list

$(IMG) : $(ELF)
	$(OBJCOPY) $(ELF) -O binary $@

$(HEX) : $(ELF)
	$(OBJCOPY) $^ -O ihex $(HEX)

flash : install

install:
	cp $(IMG) /media/pi/50BD-6FEC
