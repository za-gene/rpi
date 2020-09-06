CXX = avr-g++
MCU=atmega32
CXXFLAGS = -g -Os -Wall -mcall-prologues -mmcu=$(MCU)
BASE=app

.PHONY: install clean

$(BASE).hex : $(BASE).elf
	avr-objcopy -R .eeprom -O ihex $^ $@

$(BASE).elf : main.o
	$(CXX) -g -mmcu=$(MCU) $^ -o $@ 

.o :.cc
	$(CXX) -c $^ 


install:$(BASE).hex
	install-hex $<

clean :
	rm -f *.hex *.obj *.o *.elf

