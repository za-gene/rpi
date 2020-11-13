#ARMGNU ?= arm-none-eabi
ARMGNU ?= arm-linux-gnueabihf

USPIHOME   = ../..


LIBS	= $(USPIHOME)/lib/libuspi.a \
	  $(USPIHOME)/env/lib/libuspienv.a

AOPS = --warn --fatal-warnings 
COPS =  -nostdlib -nostartfiles -ffreestanding \
       -I../../uspi/env/include \
       -I../../uspi/include \
       -I../..
#-mfloat-abi=hard -mfpu=neon \

IMG = kernel7.img
ELF = kernel7.elf
HEX = kernel.hex

#BUILT = font.psf.h font.sfn.h
$(IMG) : $(HEX)

OBJS += kernel.o ../../vectors.o ../../interrupts.o ../../lfb.o  ../../font.psf.o ../../font.sfn.o \
	../../memory.o ../../mbox.o # gpio.o delays.o mini_uart.o uart.o string.o mbox.o interrupts.o lfb.o

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

#kernel.o : kernel.c
#	$(ARMGNU)-gcc $(COPS) -c $^ -o $@

#font_psf.o: font.psf
#	$(ARMGNU)-ld -r -b binary -o font_psf.o font.psf

#font_sfn.o: font.sfn
#	$(ARMGNU)-ld -r -b binary -o font_sfn.o font.sfn

#font.psf.c : font.psf
#	xxd -i $^ $@

#font.sfn.c : font.sfn
#	xxd -i $^ $@

#font.sfn.o : font.sfn.h
#	$(ARMGNU)-gcc $(COPS) -c $^ -o $@

#font.psf.o : font.psf.h
#	$(ARMGNU)-gcc $(COPS) -c $^ -o $@

LINKER = ../../linker.ld
#LIBUSPI = ../../uspi/lib/libuspi.a ../../uspi/env/lib/libuspienv.a


$(ELF) : $(LINKER) $(OBJS) $(FONTSO) $(LIBUSPI)
	$(ARMGNU)-ld $(FONTSO)  $(OBJS) $(LIBUSPI) -T $(LINKER)  -o $@
	$(ARMGNU)-objdump -D $@ > kernel.list

$(IMG) : $(ELF)
	$(ARMGNU)-objcopy $(ELF) -O binary $@

$(HEX) : $(ELF)
	$(ARMGNU)-objcopy $^ -O ihex $(HEX)

