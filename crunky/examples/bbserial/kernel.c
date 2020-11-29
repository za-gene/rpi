#include <async.h>
#include <basal.h>
#include <delays.h>
#include <gpio.h>
#include <stdbool.h>

#if 0

/* GPIO19 is physical pin 35, GPIO26 is physical pin 37
*/

#define pina 19
#define pinb 26

typedef struct {uint64_t start; uint64_t duration;} timer_t;

timer_t timera, timerb;

void start_timer(timer_t* t, int duration)
{
	t->start = get_system_timer(); // this is in microseconds
	t->duration = duration; // in millis
}

bool timer_expired(timer_t* t)
{
	return (get_system_timer() - t->start) > t->duration *1000;
}

#endif


typedef struct {u64 start; u64 freq; u64 ticks;} metro_t;

void start_metro(metro_t* m, u64 freq)
{
	m->start = get_system_timer();
	m->freq = freq;
	m->ticks = 0;
}

void wait_metro(metro_t* m)
{
	u64 ticks;
	while(1) {
		ticks = m->freq * (get_system_timer() - m->start)/250000000;
		if(ticks > m->ticks) break;
	}
	m->ticks = ticks;
}


void gpio_write(int bcm_pin, int value)
{
	if(value != 0)
		gpio_set(bcm_pin);
	else
		gpio_clr(bcm_pin);
}


static metro_t m;

void pause()
{
#if 1
	wait_metro(&m);
#else
	wait_us(105);
#endif
}

void kernel_main()
{
	const int tx = 6;
	gpio_sel(tx, OUTPUT);
	gpio_set(tx);

	const u64 freq = 9600;

	start_metro(&m, freq);
	for(int i = 0; i< 20; i++)
		pause();



	for(char i = 0; i< 6; i++) {
		int c = i + '0';
		start_metro(&m, freq);
		gpio_clr(tx); // send start bit
		pause();
		for(int b=0; b<8; b++) {
			gpio_write(tx, c & 1);
			c = (c>>1);
			pause();
		}
		gpio_set(tx); // send stop bit
		pause();
		pause();
	}



	while(1);
}
