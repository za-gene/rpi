main.elf : main.adb
	alr build
	mv main main.elf
	arm-none-eabi-objcopy -O binary main.elf main.bin
	arm-none-eabi-objdump -d main.elf >main.dis

clean : 
	rm -f *.o *.ali *.elf *.uf2 *.bin *.dis b__main.* main.adb.stderr main.adb.stdout
	rm -rf alire

flash :
	$(PICO_SDK_PATH)/build/elf2uf2/elf2uf2 main.elf main.uf2
	cp main.uf2 /media/$(USER)/RPI-RP2

