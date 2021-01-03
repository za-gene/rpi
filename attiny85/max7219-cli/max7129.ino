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
