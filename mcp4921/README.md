# MCP4921

It is a 12-bit audio DAC interfacing with SPI at max rate of 20MHz.

![](mcp4921.jpg)

## `dac_write()` function

The `dac_write()` function can be typically implemented as follows:
```
void dac_write(uint16_t value) {
  //value &= 0b0000111111111111;
  if(value>4095) value=4095;
  value |= 0b0011000000000000;
  SPI.beginTransaction(dac_spi_settings);
  digitalWrite(DAC_CS, LOW);
  SPI.transfer16(value);
  digitalWrite(DAC_CS, HIGH);
  SPI.endTransaction();
}
```
where there `dac_spi_settings` is defined through:
```
SPISettings dac_spi_settings(20e6, MSBFIRST, SPI_MODE0);
```
or something similar. The MCP4921 has a 20MHz max SPI clock frequency, and it is as well to try to maximise throughput. If you just use the default speed for SPI communication, then you may find that data transfer takes a surprisingly long amount of time. I found, for example, that on an ESP32, using the default settings, the transfer did not take place within the alloted timeframe for the interrupt (at 8kHz), which caused the watchdog to panic. 

You can define `DAC_CS`, which is the Chip Select pin, to be some pin of your choosing. Check the schematics in a project that you are interested in for the default settings.

You may be wondering what the following code is about:
```
  value &= 0b0000111111111111; // line 1
  value |= 0b0011000000000000; // line 2
```
Most tutorials break the transmission information into two bytes: a high byte, and a low byte. Since the Arduino has an `SPI.transfer16()` function, I figured that they way I presented it would be easier to understand.

The DAC expected a 16-bit word to be sent to it. The first 4 bits are a command, whilst the remaining 12 bits represents the value that you want to set the output to. 

So, in line 1, the input is truncated to the lower 12 bits. In line 2, the actual command bits are set. You can read more about the commands in the [datasheet](http://ww1.microchip.com/downloads/en/DeviceDoc/21897B.pdf) (PDF). Page 18 (register 15-1) is what you wnat to look at. 

Bit 15 (which writes to either DACB or DACA) has been set to 0. This is only relevant to the MCP4922, which has two channels of output (for stereo), and is irrelevant for the MCP4911. 

Bit 14 is for buffering. I think it has to to do with synchronisation for both channels, which is again irrelevant for the MCP4921. We want to set the value to 0, so that we output our value straight away.

Bit 13 is the output gain; not sure what that does exactly. We set it to 1.

Bit 12 is ¬SHDN, the output power down control bit. We set it to 1.

The ¬LDAC pin (pin 5) also seems related to buffering. We tie it to GND so that we output values straight away.

Some of the details are a little fuzzy in my mind, but what I have should work.

## Pinout

MCP4921 connections are in [nano-sdcard](nano-sdcard).

See also db5.177 for stm32


## In this directory

* `esp32-led`: basic check on MCP4921 using an LED on the device's output pin (i.e. pin 8)
* [nano-sdcard](nano-sdcard) Arduino Nano (or Uno), SD card, interrupt-driven, using MC4921. **Recommended** Works better than the WROOM
* [sketch_jun13b](sketch_jun13b) WROOM, hard-coded, interrupt-driven, using both MCP4921 and internal DAC.
* `stm-led`: as esp32-led, but for STM32, and using alternate SPI
* `stm-white`: modified `stm-led` to produce white noise
