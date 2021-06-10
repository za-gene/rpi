cmake_minimum_required(VERSION 3.12)

#set(CMAKE_BUILD_TYPE Debug)
#set(PICO_CXX_ENABLE_EXCEPTIONS 1)

include("../picomc.cmake")
project(app C CXX ASM)
pico_sdk_init() # Initialize the SDK

add_executable(app
        picol.c
        )

pico_enable_stdio_uart(app 1)
pico_enable_stdio_usb(app 0)

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(app pico_stdlib hardware_spi)

pico_add_extra_outputs(app) # create map/bin/hex file etc.

