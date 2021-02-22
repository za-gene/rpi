# audio

## Conversion

Note that sox cannot handle mp3 files, so you need to convert them to wav first.

**mp3 to wav**:
```
ffpmeg -i song.mp3 song.wav
```
I found that a 1.7M mp3 at 44.1kHz file lasting 1m09s expanded to a 12.3M wav file.

**wav to raw**:
```
sox song.wav -e unsigned -b 8 -r 8k -c 1 song.raw
```
Reduced the wav file to 560k. `-b` is bytes per sample, `-r` is rate,
`-c` is the channel number

**play it**:
```
aplay song.raw
```


## In this directory

* [freqs](freqs) - list of frequencies
* [pwm-audio-sdcard-nano](pwm-audio-sdcard-nano) - Nano RAW player using PWM and sound card
* [sketch_jun09a](sketch_jun09a) - Arduino emulating DAC using PWM - poor output quality
* [sketch_jun18](sketch_jun18) - Arduino slave with 5v SD card communicating via I2C with ESP32 master using built-in DAC. Uses a level shifter. 
* [sync8000](sync8000) - Produces a pulse at 8kHz. Designed as a DAC metronome. Uses ATTiny85.



## Music Tracks

A good song that I used for `sketch_jun9a` is available for download [here](https://freemusicarchive.org/music/BoxCat_Games/Nameless_the_Hackers_RPG_Soundtrack/BoxCat_Games_-_Nameless-_the_Hackers_RPG_Soundtrack_-_10_Epic_Song). It is a good song. A [direct download](https://drive.google.com/file/d/1fU7viQhGI5p9GGPTPLHQ3CrVoz7BrtE9/view?usp=sharing) converted to a mono 8KhHz is also available.

## Unclassified

**YX5300** is an MP3 player with SD card and audio jack. Some have volume controls

## Also on this site

* [dev09](../dev09) - sine wave and consant voltage generator
* [tone440](../attiny85/tone440) - squarewave on attiny85

