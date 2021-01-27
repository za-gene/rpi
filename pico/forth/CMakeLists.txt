add_executable(forth
        forth.cpp
        )

# Pull in our pico_stdlib which aggregates commonly used features
target_link_libraries(forth pico_stdlib)

# enable usb output, disable uart output
#pico_enable_stdio_usb(forth 1)
pico_enable_stdio_uart(forth 1)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(forth)

# add url via pico_set_program_url
example_auto_set_url(forth)
