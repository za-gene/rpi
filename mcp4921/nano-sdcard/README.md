# nano-sdcard

Turn your Nano (or Uno) into am iPod. Play an 8-bit 8kHz mono song (raw format) stored on an SD card (as `raw.song`) through an MCP4921 (a 12-bit DAC) and speaker.

## Schematics

![](mplayer.png)





<img src="physical.jpg"  width="400">

## Discussion

Put a mono 8kHz song onto an SD card as `raw.song`. Click [here](https://drive.google.com/file/d/1fU7viQhGI5p9GGPTPLHQ3CrVoz7BrtE9/view?usp=sharing) to download a sample. The orginal song is available under the Creative Commons Attribution License  [here](https://freemusicarchive.org/music/BoxCat_Games/Nameless_the_Hackers_RPG_Soundtrack/BoxCat_Games_-_Nameless-_the_Hackers_RPG_Soundtrack_-_10_Epic_Song). The author is BoxCat Games, and the name of the pice is Epic Song.

Connect a Nano (or Uno) with an [SD card module](https://www.ebay.co.uk/itm/Micro-SD-Card-Mini-TF-Module-SPI-Interfaces-with-Converter-Chip-for-Ardunio-MCU/361514788210?hash=item542bf5e572:g:hvcAAOSwj-BbA~b~)  and an [MCP4921](https://www.ebay.co.uk/itm/MICROCHIP-MCP4921-E-P-12BIT-DAC-SINGL-SPI-I-F-MCP4921/171424274463?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2057872.m2749.l2649) (12-bit DAC) according to the schematics shown above. 

This setup shown in this project is better than using PWM, discussed [here](https://github.com/blippy/rpi/tree/master/audio/sketch_jun09a). It even seems to perform better than the [version](https://github.com/blippy/rpi/tree/master/audio/sketch_jun18) for the ESP32. 

In the schematic above, connect the VOUT of the MCP4921 to one terminal of a speaker. The other terminal on the speaker should, of course, be connected to ground. I couldn't actually hear anything when I plugged the speaker in directly, so you will probably have to connect the speaker via an OPAMP.

The pin layout and nano-side connections for the MCP4921 is as follows:
```
NANO           MCP4921
              ---------
              |   U   |
          VDD-| 1   8 |-VOUTA   -> OPAMP -> SPEAKER -> GND
D2   ->    CS-| 2   7 |-AVSS    -> GND
D13  ->   SCK-| 3   6 |-VREFA   -> VDD
D11  ->  MOSI-| 4   5 |-LDAC    -> GND
              |       |
              ---------
```
    
This project uses timer interrupts to time the output. TIMER2 is used for this purpose, which is typically used for tone() generation.  `init_timer2()` sets the appropriate timer registers, which is discussed in-depth [here](https://github.com/blippy/rpi/tree/master/timer). 

The interrupt code is straightforward:
```
ISR(TIMER2_COMPA_vect) {
  if (!dataFile.available()) {
    dataFile.seek(0);
  }
  dac_write(dataFile.read() << 4); // convert a byte value to a 12-bit value
}
```

If we reach the end of the data file, then we just move back to the start. Otherwise, we call `dac_write()` (discussed [here](https://github.com/blippy/rpi/tree/master/mcp4921)) with a byte from the file. `dataFile.read()` returns an unsigned byte (8 bits) which we scale up to 12 bits using `<<4` on account of the fact that the MCP4921 is a 12-bit DAC.

## See also

* [Youtube video](https://youtu.be/sXhWhjDb8Fk) of device in action


