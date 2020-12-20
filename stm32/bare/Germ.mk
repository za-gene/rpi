include $(STM32)/Settings.mk


AOPS = --warn --fatal-warnings 
#COPS =  -nostdlib -nostartfiles -ffreestanding -I$(CRUNKY)



#$(IMG) : $(HEX)

#LIB_CRUNKY = libcrunky.o

#$(LIB_CRUNKY) : $(CRUNKY_OBJS)

all : app.bin

clean :
	rm -f *.o *.bin *.hex *.elf *.list *.img *.a $(BUILT)


%.o : %.c
	$(CC) $(CFLAGS) -c $^ -o $@

%.o : %.cc
	$(CXX) $(CXXFLAGS) -c $^ -o $@

LINKER = $(STM32)/linker.ld


#XCEPT = -L /usr/lib/arm-none-eabi/newlib -lsupc++
NEWLIB = -L/usr/lib/arm-none-eabi/newlib/hard/ -lc_nano

$(ELF) : $(LINKER) $(OBJS)  
	$(LD)   $(OBJS) -T $(LINKER)  -L$(STM32) -lblue $(NEWLIB) -o $@
	#$(OBJDUMP) -D $@ > $(KERNEL).list

#$(IMG) : $(ELF)
#	$(OBJCOPY) $(ELF) -O binary $@

#$(HEX) : $(ELF)
#	$(OBJCOPY) $^ -O ihex $(HEX)

app.bin: app.elf
	$(BIN) -O binary app.elf app.bin
	arm-none-eabi-objdump -d app.elf >app.list



.flash: app.bin
	touch .flash

flash : .flash
	$(STL) write app.bin 0x8000000

erase:
	$(STL) erase
