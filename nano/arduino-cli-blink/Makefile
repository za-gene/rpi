FQBN = arduino:avr:nano
FQBN1 = $(subst :,.,$(FQBN))	
BDIR = build/$(FQBN) # build dir
HERE = $(shell basename $(CURDIR))
INO  = $(HERE).ino
HEX  = build/$(FQBN1)/$(INO).hex
BIN  = build/$(FQBN1)/$(INO).bin
PORT = $(shell ls /dev/ttyUSB*)


#hex : $(HEX)

$(BIN) : $(INO)
	arduino-cli compile --fqbn $(FQBN) -e 

install : 
	echo port is $(PORT)
	#arduino-cli upload -b $(FQBN) -i $(BIN) -p $(PORT)
	arduino-cli upload -b $(FQBN):cpu=atmega328old -i build/arduino.avr.nano/arduino-cli-blink.ino.hex -p $(PORT)
