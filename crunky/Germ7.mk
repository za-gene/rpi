include $(CRUNKY)/Settings.mk


AOPS = --warn --fatal-warnings 
COPS =  -nostdlib -nostartfiles -ffreestanding -I$(CRUNKY)



#BUILT = font.psf.h font.sfn.h
$(IMG) : $(HEX)

#CRUNKY_OBJS = vectors.o interrupts.o lfb.o  font.psf.o font.sfn.o \
#	memory.o mbox.o  gpio.o delays.o uart.o string.o # mini_uart.o

LIB_CRUNKY = libcrunky.o

$(LIB_CRUNKY) : $(CRUNKY_OBJS)

#FONTSO = font.psf.o font.sfn.o

all : $(IMG)

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f $(BUILT)

#vectors.o : vectors.s
#	$(ARMGNU)-as vectors.s -o vectors.o

%.o : %.c
	$(ARMGNU)-gcc $(COPS) -c $^ -o $@


LINKER = ../../linker.ld


$(ELF) : $(LINKER) $(OBJS) $(FONTSO) $(LIBUSPI) vectors.o
	$(ARMGNU)-ld vectors.o $(FONTSO)  $(OBJS) $(LIBUSPI) -T $(LINKER)  -L$(CRUNKY) -lcrunky -o $@
	$(ARMGNU)-objdump -D $@ > kernel.list

$(IMG) : $(ELF)
	$(ARMGNU)-objcopy $(ELF) -O binary $@

$(HEX) : $(ELF)
	$(ARMGNU)-objcopy $^ -O ihex $(HEX)

flash : install

install:
	cp $(IMG) /media/pi/50BD-6FEC
