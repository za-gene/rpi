#include <async.h>
#include <basal.h>
#include <timers.h>
#include <gpio.h>
#include <stdbool.h>



typedef struct {u64 start; u64 freq; u64 ticks;} metro_t;

void start_metro(metro_t* m, u64 freq)
{
	m->start = get_system_timer();
	m->freq = freq;
	m->ticks = 0;
}

void wait_metro(metro_t* m)
{
#if 0
	u64 ticks;
	while(1) {
		ticks = m->freq * (get_system_timer() - m->start)/1000000;
		if(ticks > m->ticks) break;
	}

	u64 t = get_system_timer();
#endif
	m->ticks++;
	//while(get_system_timer() - m->freq * m->ticks *1000000 <  m->start);
	while(get_system_timer() - m->ticks *1000000/m->freq <  m->start);
	//while(get_system_timer() < m->ticks 
	//m->ticks = ticks;
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
	const int tx = 13;
	gpio_sel(tx, OUTPUT);
	gpio_set(tx);

	const u64 freq = 6000*2;
	start_metro(&m, freq);
	while(1) {
		wait_metro(&m);
		gpio_toggle(tx);
	}


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
