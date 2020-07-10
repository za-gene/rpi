# dev08 

Squarewave generator designed for 480Hz output (untested). The circuit uses an **NE555** 

## Schematics

<img src="dev08.svg" width="600">

Internal components:
```
C1 0.1uF capacitor
C2 0.01uF capacitor
R1 10K resistor
R2 10K resistor
```

External connections:

```
1 Vout (oscillates 0V - Vin)
2 Not connected
3 GND
4 Not connected
5 Vin (5V-12V)
```

## Example circuit

Here's an example of how you might hook-up the generator

<img src="dev08eg.svg" width="400">

**Important**: Failure to use a resistor causes the 555 to overheat; a proper tone is not generated.


## Theory

The frequency of oscillation off the 555 timer is:

f = 1.44/(C1(R1+2R2))

Given C1= 0.1uF, R1=R2=10K produces a theoretical frequency of 480Hz. 

## References

* [How does NE555 timer circuit works](https://www.eleccircuit.com/555-timer-circuit-works/)

