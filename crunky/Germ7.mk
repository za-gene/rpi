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

%.o : %.cc
	$(CXX) $(CXXFLAGS) -c $^ -o $@

LINKER = $(CRUNKY)/linker.ld


NEWLIB = -L/usr/lib/arm-none-eabi/newlib/hard/
#NEWLIB += -lstdc++_nano  -lsupc++_nano
NEWLIB += -lc_nano
#NEWLIB += -lg
#NEWLIB += -lg_nano
#GCCLIB = -L/usr/lib/gcc/arm-none-eabi/7.3.1/hard -lgcc

$(ELF) : $(LINKER) $(OBJS)  $(LIBUSPI) $(CRUNKY)/vectors.o
	$(LD) $(CRUNKY)/vectors.o   $(OBJS) -T $(LINKER)  -L$(CRUNKY) -lcrunky $(LUALIB) $(NEWLIB)  ../../nanolib-impl.o  $(GCCLIB) -lm -o $@
	$(OBJDUMP) -D $@ > $(KERNEL).list

$(IMG) : $(ELF)
	$(OBJCOPY) $(ELF) -O binary $@

$(HEX) : $(ELF)
	$(OBJCOPY) $^ -O ihex $(HEX)

flash : install

install:
	cp $(IMG) /media/pi/50BD-6FEC
