#define F_CPU 1000000UL

#define PIN PB4

void setup()
{
	pinMode(PIN, OUTPUT);
}

void loop()
{
	static uint8_t i = 0;
	analogWrite(PIN, i++);
	delay(20);
}



