No, the pico can be readied for new code by either software or hardware.

Your software would need to be compiled with PICO_ENTER_USB_BOOT_ON_EXIT then a condition that ends the main loop will set the Pico back, ready to load new software.

A hardware solution would be to reset the Pico by pulling the run pin (pin 30) low and either hold the BOOTSET button down when releasing pin 30 or pulling down TP6 until after pin 30 is released.
Having a single physical button that sets the Pico into a Boot ready mode would risk occasional accidental boot conditions, something that would receive a lot of criticism I'm sure.

