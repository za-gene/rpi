# baby 8 sequencer

A baby-8 consists of 8 "channels", where each channel is assigned an audio frequency. 
The sequencer's stepper cycles through each channel in turn, playing the note for that
channel. It then returns to the beginning.
The cycling speed has a default of 1 sec per channel, which can be changed.

The sequencer is controlled over the serial port (GP0 for UART0 TX, GP1 for RX) at a baud of 115200.
A crude interpreter is provided to instruct the configuration of the output.

A [YouTube](https://youtu.be/Bj0u3xA7q0w) video demonstrates the action and operation of the sequencer.


## Components
* Raspberry Pi Pico
* Small speaker. A 4 ohm 2W speaker is suitable, as is a wide range of other speakers.
* a blinkt, which consists of eight RGB (APA102) LEDs in a single module that is
controlled by a SPI-like interface. It is used to indicate which channel is playing.
As an alternative, 8 LEDs could be used. This would require program modification, though.

## Configuration

Speaker:
* attach one terminal of the speaker to GP14
* attach the remaining terminal to GND

Blinkt: Pimoroni blinkt [modules](https://shop.pimoroni.com/products/blinkt) were designed 
to work with Raspberry Pi 3's and above. Internal connections are described 
[here](../blinkt) (Wiring section). The connections required are:
* pin 2 of blinkt should be connected to Pico's VBUS pin
* pin 6 should be connected to any ground pin of the Pico
* pin 16 (data pin) of blinkt should be connected to Pico's GP16
* pin 18 (latch pin) should be connected to GP16

C++ [code](blinkt.cc) is in this directory, as is the [header](blinkt.h) file.

## Controlling the sequencer

The interpreter is crude, and styled on Basic. The main interpreter is in the file `abba.cc`.
It can be compiled to a native Linux binary on any platform, and has a very limited command set
in its own right. This is supplemented by a set of commands aimed at the controlling the 
sequencer. They are implemented in the file `abba-pico.cc`. See the directory
[abba](abba) for the complete project.

Build the software in the sequencer from the `abba` directory in the usual way for the Pico.

Connect a serial communication program (e.g. picocom) to the Pico. The command are 
described below.

**`baby info`**

Print information about the duration of each stop and the channel frequencies.


**`baby <c> <f>`**

Set the frequency of channel `c` to `f` Hz. There are 8 channels labelled 1..8 inclusive.


**`baby step <s>`**

This changes the speed of the sequencer so that each channel is output for `s` milliseconds.


**`baby off`**

Sound is turned off.


**`baby on`**

Sound is turned on.


## In conclusion

If you have any questions on the sequencer post them on 
[this](https://www.reddit.com/r/raspberry_pi/comments/m2ujc1/a_baby_8_sequencer_for_the_pico/) reddit thread.

