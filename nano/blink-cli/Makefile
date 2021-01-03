FQBN =arduino:avr:nano
FQBN1 =$(subst :,.,$(FQBN))	
FQBN1:=$(strip $(FQBN1))
BDIR =build/$(FQBN)
HERE =$(notdir $(CURDIR))
INO  =$(HERE).ino
HEX  =build/$(FQBN1)/$(INO).hex
BIN  =build/$(FQBN1)/$(INO).bin
PORT = $(shell ls /dev/ttyUSB*)


#hex : $(HEX)

$(BIN) : $(INO)
	echo bin is $(BIN)
	arduino-cli compile --fqbn $(FQBN) -e 

flash : 
	echo port is $(PORT)
	arduino-cli upload -b $(FQBN):cpu=atmega328old -i $(HEX) -p $(PORT)

clean :
	rm -rf build
