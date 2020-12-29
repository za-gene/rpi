# stm8 pin test

This is a test on pins PB4 and PB5. You should probably just ignore this project.

It turns out that PB4/5 are "true sinks", meaning that driving the pins high won't help. I think we need pull-up resistors to that end.

Those pins are for I2C.
