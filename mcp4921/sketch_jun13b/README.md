# sketch_jun13b

MCP4921 12-bit audio DAC for WROOM. This project can be configured to use either the WROOM's internal DAC (on D25) or the external DAC (MCP4921) for comparison purposes.

## Connection

### Using Internal DAC

Connect a speaker between pin D25 and ground. Uncomment the line:
```
//#define USE_INTERNAL_DAC
```
and compile.

### Using the MCP4921

The pin layout is as follows:
```
WROOM          MCP4921
              ---------
              |   U   |
          VDD-| 1   8 |-VOUTA   -> SPEAKER -> GND
D5   ->    CS-| 2   7 |-GND
D18  ->   SCK-| 3   6 |-VDD
D19  ->  MOSI-| 4   5 |-GND
              |       |
              ---------
```

In other words, connect pin D5 on WROOM to CS on the MCP4921, D18 to SCK and D19 to MOSI.
GND is Ground, and VDD is 3V3 supply from the WROOM. VOUTA is the voltage output. Connect it to a speaker (which in turn is connected to GND) or to a speaker via an OPAMP.


