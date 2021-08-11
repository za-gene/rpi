# 2021-06-29 created
# The creation of interfaces is discussed here:
# https://www.raspberrypi.org/forums/viewtopic.php?f=145&t=306204&p=1845890#p1845890
# An example project which uses the interface is 'timer_lowlevel'

add_library(pi INTERFACE)

target_sources(pi INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/blinkt.cc
	${CMAKE_CURRENT_LIST_DIR}/debounce.cc
	${CMAKE_CURRENT_LIST_DIR}/fat32.cc
	${CMAKE_CURRENT_LIST_DIR}/ledmat.cc
	${CMAKE_CURRENT_LIST_DIR}/mcp4921-dma.cc
	${CMAKE_CURRENT_LIST_DIR}/notes.c
	${CMAKE_CURRENT_LIST_DIR}/pace.c
	${CMAKE_CURRENT_LIST_DIR}/pi.cc
	${CMAKE_CURRENT_LIST_DIR}/picol.c
	${CMAKE_CURRENT_LIST_DIR}/pulse.cc
	${CMAKE_CURRENT_LIST_DIR}/rotary.cc
	${CMAKE_CURRENT_LIST_DIR}/sdcard.cc
	${CMAKE_CURRENT_LIST_DIR}/ssd1306.cc
)

target_include_directories(pi INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(pi INTERFACE 	
	pico_stdlib
	hardware_dma
	hardware_gpio
	hardware_i2c
	#hardware_irq
	hardware_pwm
	hardware_spi
)

target_compile_definitions(pi INTERFACE)
