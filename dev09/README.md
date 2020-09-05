# dev09: voltage generator. esp32 mcp4921 simplecli

## Prerequisites

Simplecli:

(cd ~/Ardiuno/libraries ; git clone https://github.com/spacehuhn/SimpleCLI)

## Commands

### `setv value`

Set a voltage value: 0-4096. 4096~3V3. Example:
```
setv 4096
```
to set the voltage output to maximum.


## See also

* db05.193 for circuit laout. Signal comes out on left connector. GND is on right.
