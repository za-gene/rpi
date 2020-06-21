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
D5   ->    CS-| 2   7 |-AVSS    -> GND
D18  ->   SCK-| 3   6 |-VREFA   -> VDD
D23  ->  MOSI-| 4   5 |-LDAC    -> GND
              |       |
              ---------
```

In other words, connect pin D5 on WROOM to CS on the MCP4921, D18 to SCK and D19 to MOSI.
GND is Ground, and VDD is 3V3 supply from the WROOM. VOUTA is the voltage output. Connect it to a speaker (which in turn is connected to GND) or to a speaker via an OPAMP.

MISO (which is traditionally D19 on WROOM) is not required. The pins D5, D18, D19, D23 constitute the VSPI interface which is the default when using the Arduino SPI library for the WROOM.


## Discussion

The MCP4921 has a max SPI clock frequency of 20MHz. It is important to ensure that you use a fast speed. The default speed for SPI seems too low on the WROOM, making the interrupt take too long to execute, and causing a **watchdog reset**. `onTimer()` is an interrupt that executes every 125us (corresponding to an audio frequency of 8kHz. 1/8000 = 125us). It, in turn, executes `dac_write()`, which does the SPI transfer to the MCP4921.

The line
```
SPISettings spi_settings(20e6, MSBFIRST, SPI_MODE0);
```
therefore ensures that we're executing the transfers as quickly as possible. Those settings are encapulated in `dac_write()` by the code:
```
  SPI.beginTransaction(spi_settings );
  digitalWrite(DAC_CS, LOW);
  SPI.transfer16(value);
  digitalWrite(DAC_CS, HIGH);
  SPI.endTransaction();
```

`dacwrite()` has the protoype 
```
void IRAM_ATTR dac_write(uint16_t value) 
```
The `IRAM_ATTR` pragma puts the code into IRAM memory, as opposed to being retrieved from Flash. This is a speed-up that is desirable for use in interrupts.

The interrupt is set up within `setup()` as follows:
```
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1e6 / 8000, true);
  timerAlarmEnable(timer);
```  
The `80` is a prescaler which divides up the clock cycle. A WROOM executes at 80MHz, so dividing the clock by 80 means that we are measuring in units of 1MHz. The `1e6/8000` splits up this 1MHz at a frequency of 8kHz, a common audio transmission rate.
