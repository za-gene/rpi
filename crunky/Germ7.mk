include $(CRUNKY)/Settings.mk


AOPS = --warn --fatal-warnings 
#COPS =  -nostdlib -nostartfiles -ffreestanding -I$(CRUNKY)



$(IMG) : $(HEX)

LIB_CRUNKY = libcrunky.o

$(LIB_CRUNKY) : $(CRUNKY_OBJS)

all : $(IMG)

clean :
	rm -f *.o *.bin *.hex *.elf *.list *.img *.a $(BUILT)


%.o : %.c
	$(CC) $(COPS) -c $^ -o $@


LINKER = $(CRUNKY)/linker.ld


$(ELF) : $(LINKER) $(OBJS)  $(LIBUSPI) $(CRUNKY)/vectors.o
	$(LD) $(CRUNKY)/vectors.o   $(OBJS) -T $(LINKER)  -L$(CRUNKY) -lcrunky -o $@
	$(OBJDUMP) -D $@ > $(KERNEL).list

$(IMG) : $(ELF)
	$(OBJCOPY) $(ELF) -O binary $@

$(HEX) : $(ELF)
	$(OBJCOPY) $^ -O ihex $(HEX)

flash : install

install:
	cp $(IMG) /media/pi/50BD-6FEC
