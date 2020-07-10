# dev08 

Squarewave generator designed for 440Hz output, but actually appears to be around 390Hz. Uses an **NE555** 

## Schematics

<img src="dev08.svg" width="600">

Internal connections:
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

## Example connection

Here's an example of how you might hook-up the generator

<img src="dev08eg.svg" width="400">

**Important**: Failure to use a resistor causes the 555 to overheat; a proper tone is not generated.


## Theory

The frequency of oscillation off the 555 timer is:

f = \frac{1.44}{C1(R1+2R2)}
