OBJS = main.o #gpio.o

include $(ATTINY85)/t85.mk

#CXX = avr-g++
#CXXFLAGS = -g -Os -Wall -mcall-prologues -mmcu=attiny85 -std=c++14
#BASE=app

#.PHONY: install clean

#$(BASE).hex : $(BASE).elf
#	avr-objcopy -R .eeprom -O ihex $^ $@
#	avr-objdump -D $^ >app.list

#$(BASE).elf : main.o 
#	$(CXX) -g -mmcu=attiny85 $^ -o $@ 

#.o :.cc
#	$(CXX) -c $^ 


#flash : install

#install:$(BASE).hex
#	install-hex $<

#clean :
#	rm -f *.hex *.obj *.o *.elf *.list

