# 4 device


Install libraries:
```
git clone git@github.com:blippy/lcddogmspi.git
```

Low-level controllers:
* `lcdard` - Nano/Uno/WROOM. Connect as per Layout section
* `lcdesp.py` - ESP8266
* `lcdpi.py` - RPi3. Use this is as a baseline

**Better**: use DogLcd. `lcddomspi` has problems with cursors.

```
git clone git@github.com:blippy/DogLcd.git
```

## Layout

![](4.jpg)

Pins are labelled C1, C2, ... C6 left to right in the diagram above.

```
DEV  DESC   WROOM
C1   5V     VIN
C2   RS[1]  D4  
C3   CS     D5
C4   CLK    D18
C5   MOSI   D23
C6   GND    GND
```

[1] **MISO** is not used. **RS** is set low to issue a command, high to issue an ASCII. DO NOT use a regular MISO on ESP8266, use an alternative pin like D4 (GPIO2).


## See also

* [Datasheet](https://www.lcd-module.com/eng/pdf/doma/dog-me.pdf) EA DOGM163W-A
* [Display-O-Tron LCD](https://shop.pimoroni.com/products/display-o-tron-lcd?gclid=EAIaIQobChMI1qe7iZmU4gIVQpnVCh2DhA0rEAQYAiABEgK1HfD_BwE&utm_campaign=google+shopping&utm_medium=cpc&utm_source=google&variant=2662374913)
* [lcd-clock-esp.py](https://gist.github.com/blippy/ea7709597cae84c3bcde02d2bc18b1e5) includes fancy LCD commands
* [lcddogmspi](https://github.com/blippy/lcddogmspi) -  for Arduino (a working fork of the original)
