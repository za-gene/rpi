# pi c

Testing of mcp4921 where you set levels manually.

## Results

```
level   voltage     amps
    0     1.1mV      9uA
  100    80.2mV   4.54mA
  200     161mV    9.0mA
  250     200mV   11.2mA
  400     321mV   13.8mA
  500     402mV   13.6mA
 1000     805mV   13.5mA
 2000     1.61V   13.5mA
 4000     3.21V   13.5mA
 4095     3.29V   13.5mA
```

There is a close match between measured voltage and theoretical voltage.


## Connection

```
MCP INFO PI
1   3V3   1
2   CS   24
3   SCK  23
4   MOSI 18
5   GND   6
6   3V3   1
7   GND   6
8   VOUT  -

COLS:
* MCP - pin connection to MCP4921
* INFO - description. VOUT goes to amp, not Pi
* PI - physical pin for Pi connection
```

## NEWS

2021-03-02 Started.  Working
