cmake_minimum_required(VERSION 3.12)
include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

project(app C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

#set(PICO_CXX_ENABLE_EXCEPTIONS 1)

pico_sdk_init()
include_directories(..)

add_executable(app
        main.cc
        #../bslinkt.cc
        )

pico_enable_stdio_usb(app 1)

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(app 
	pico_stdlib 
	#hardware_clocks
	hardware_gpio
	#hardware_pwm
	#hardware_spi
	)

pico_add_extra_outputs(app) # create map/bin/hex file etc.

add_custom_target(flash
    COMMAND cp app.uf2 /media/pi/RPI-RP2
    DEPENDS app
)

