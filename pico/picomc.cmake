message("picomc.cmake says hi")


#project(app C CXX ASM) # needs to be in the projects file, I think
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)



include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)
include_directories(..)
#pico_sdk_init()

add_custom_target(flash
    COMMAND cp app.uf2 /media/pi/RPI-RP2
    DEPENDS app
)


