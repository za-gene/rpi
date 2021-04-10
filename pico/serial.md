# serial communication

## USB

To configure serial output for USB, ensure `CMakeLists.txt` reads:
```
...
add_executable( ... )

pico_enable_stdio_usb(app 1)
pico_enable_stdio_uart(app 0)

...
```

In order to block until the serial port connection has been established:
```
#include "tusb.h"

int main()
{
        stdio_init_all();
        while(!tud_cdc_connected()) sleep_ms(250);
        puts("Hello from RP2040");
        ...
}
```

[Reference](https://www.raspberrypi.org/forums/viewtopic.php?f=145&t=309155&p=1849507#p1849507)



