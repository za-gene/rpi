FQBN = arduino:avr:nano

FQBN1 =$(subst :,.,$(FQBN))	
FQBN1:=$(strip $(FQBN1))
BDIR =build/$(FQBN)
#HERE = $(shell basename $(CURDIR))
HERE =$(notdir $(CURDIR))
INO  =$(HERE).ino
HEX  =build/$(FQBN1)/$(INO).hex
BIN  =build/$(FQBN1)/$(INO).bin
PORT = $(shell ls /dev/ttyUSB*)
#PORT = /dev/ttyACM0


#hex : $(HEX)

$(BIN) : $(INO)
	echo bin is $(BIN)
	arduino-cli compile --fqbn $(FQBN) -e 

flash : 
	# nah - wont' load to new board
	#install-hex build/*/*.hex
	arduino-cli upload -b $(FQBN) -i $(HEX) -p $(PORT)

clean :
	rm -rf build
