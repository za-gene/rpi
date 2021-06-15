# Debounce

Debounces a button with an action taken on the falling (i.e. pushing down) edge.


## Simple debouncing on ATtiny85

```
#define BTN PB4

void button_init()
{
        pinMode(BTN, INPUT_PULLUP);
}


bool button_fell = false;


void button_poll()
{
#define MAX_COUNT 1000UL        
        static u32 count = 0;
        if(digitalRead(BTN) == HIGH) {
                count = 0;
        } else {
                count ++;
                if(count == MAX_COUNT) button_fell = true;
                if(count > MAX_COUNT) count = MAX_COUNT+1;
        }
}


setup()
{
	button_init();
	...
}


loop()
{
       button_poll();
       //static int temp = 0;
       if(button_fell) {
                button_fell = false;
		...
        }

	...
}
```

Further details in [../hertz/hertz-t85-arduino](../hertz/hertz-t85-arduino)

