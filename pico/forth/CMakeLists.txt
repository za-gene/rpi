cmake_minimum_required(VERSION 3.12)

# Pull in PICO SDK (must be before project)
#include(pico_sdk_import.cmake)
include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

project(app C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

#set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

pico_sdk_init() # Initialize the SDK

#flex_target(Scanner scanner.l lex.yy.c)

find_package(FLEX)
FLEX_TARGET(MyScanner scanner.l ../lex.yy.c)
#ADD_FLEX_BISON_DEPENDENCY(MyScanner)

add_executable(app
	lex.yy.c
        newforth.cc
	#${FLEX_MyScanner_OUTPUTS}

        )


#pico_enable_stdio_usb(app 1)
pico_enable_stdio_uart(app 1)


# Pull in our pico_stdlib which aggregates commonly used features
#target_link_libraries(app ${FLEX_LIBRARIES} pico_stdlib)
target_link_libraries(app pico_stdlib)

pico_add_extra_outputs(app) # create map/bin/hex file etc.
