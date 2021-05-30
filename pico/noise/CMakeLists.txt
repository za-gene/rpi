cmake_minimum_required(VERSION 3.12)

#set(CMAKE_BUILD_TYPE Debug)
#set(PICO_CXX_ENABLE_EXCEPTIONS 1)

include("../picomc.cmake")
project(app C CXX ASM)
pico_sdk_init()

add_executable(app
        main.cc
        #../blinkt.cc
        )

#pico_enable_stdio_usb(app 0) # set to 1 if you want stdio over usb
#pico_enable_stdio_uart(app 1) # set to 1 if you stdio over GP0 and GP1

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(app 
	pico_stdlib 
	#harware_adc
	#hardware_clocks
	#hardware_flash
	hardware_gpio
	#hardware_i2c
	#hardware_irq
	#hardware_pwm
	#hardware_spi
	)

pico_add_extra_outputs(app) # create map/bin/hex file etc.
