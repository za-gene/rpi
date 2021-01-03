# ADC

The following pins have ADC:


```
Pin 1: PB5: A0  (reset pin)
Pin 2: PB3: A3
Pin 3: PB4: A2
Pin 7: PB2: A1
```


## Arduino example


```
#define F_CPU 1000000UL

#define LED PB4 // PB4, physical pin 3
#define POT A1 	// PB2, physical pin 7

void setup()
{
	pinMode(LED, OUTPUT);
	pinMode(POT, INPUT);
}

void loop()
{
	int val = analogRead(POT);
	val = map(val, 0, 1023, 0, 255);
	analogWrite(LED, val);
}

```

Confirmed working 2021-01-03 using oscilloscope with SEN1=1V, SEN2=X2, time setting 0.1ms.

See: [adc-cli](adc-cli)


