CFLAGS = -I $(ATTINY85)
CXX = avr-g++
CXXFLAGS = $(CFLAGS) -g -Os -Wall -mcall-prologues -mmcu=attiny85 -std=c++14
VPATH = $(ATTINY85)
BASE=app
ELF=$(BASE).elf


all : $(ELF)

.PHONY: install clean

$(BASE).hex : $(ELF)
	avr-objcopy -R .eeprom -O ihex $^ $@
	avr-objdump -D $^ >app.list

$(ELF) : $(OBJS)
	$(CXX) -g -mmcu=attiny85 $^ -o $@ 

.o :.cc
	$(CXX) -c $^ 


flash : install

install:$(BASE).hex
	install-hex $<

clean :
	rm -f *.hex *.obj *.o *.elf *.list

