FQBN = attiny:avr:ATtiny85

FQBN1 =$(subst :,.,$(FQBN))	
FQBN1:=$(strip $(FQBN1))
BDIR =build/$(FQBN)
#HERE = $(shell basename $(CURDIR))
HERE =$(notdir $(CURDIR))
INO  =$(HERE).ino
HEX  =build/$(FQBN1)/$(INO).hex
BIN  =build/$(FQBN1)/$(INO).bin
PORT = $(shell ls /dev/ttyUSB*)


#hex : $(HEX)

$(BIN) : $(INO)
	echo bin is $(BIN)
	arduino-cli compile --fqbn "attiny:avr:ATtinyX5" -e 

flash : 
	install-hex build/*/*.hex
	#arduino-cli upload -b "attiny:avr:ATtinyX5" -i $(HEX)

clean :
	rm -rf build
