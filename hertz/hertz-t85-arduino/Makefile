app.hex : app.out

clean :
	rm -rf build app.out app.hex

# https://stackoverflow.com/questions/31381007/compiling-an-assembly-program-using-avr-gcc

app.out : main.c
	avr-gcc main.c -mmcu=attiny85 -Os -g -o app.out
	avr-objcopy -O ihex app.out app.hex


flash : app.hex
	install-hex app.hex
	#sudo avrdude -p t85 -P /dev/spidev0.0 -c linuxspi -b 10000 -U flash:w:app.hex

