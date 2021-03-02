# pi c

Playing a song from the Pi through MCP4921. Tested with audio jack. Tiny speakers didn't
seem to work. Power was probably very low.

There didn't appear to be any timing issues.


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

2021-03-01 Started. Working
