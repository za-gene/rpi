# 2021-06-29 created
# The creation of interfaces is discussed here:
# https://www.raspberrypi.org/forums/viewtopic.php?f=145&t=306204&p=1845890#p1845890
# An example project which uses the interface is 'timer_lowlevel'

add_library(pi INTERFACE)

target_sources(pi INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/pi.cc
)

target_include_directories(pi INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(pi INTERFACE hardware_spi)
target_compile_definitions(pi INTERFACE)
