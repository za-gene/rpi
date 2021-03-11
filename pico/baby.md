# baby 8 sequencer

A baby-8 consists of 8 "channels", where each channel is assigned an audio frequency. 
The sequencer's stepper cycles through each channel in turn, playing the note for that
channel. It then returns to the beginning.
The cycling speed has a default of 1 sec per channel, which can be changed.

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


