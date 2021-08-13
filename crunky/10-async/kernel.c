#include <async.h>
#include <delays.h>
#include <gpio.h>
#include <stdbool.h>


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



static async blinka(struct async *pt)
{
	async_begin(pt);
	while(1)
	{
		start_timer(&timera, 100);
		gpio_set(pina);
		await(timer_expired(&timera));

		start_timer(&timera, 1000);
		gpio_clr(pina);
		await(timer_expired(&timera));
	}
	async_end;
}

static async blinkb(struct async *pt)
{
	async_begin(pt);
	while(1)
	{
		start_timer(&timerb, 1300);
		gpio_set(pinb);
		await(timer_expired(&timerb));

		start_timer(&timerb, 1300);
		gpio_clr(pinb);
		await(timer_expired(&timerb));
	}
	async_end;
}


static struct async blinka_pt, blinkb_pt;

void kernel_main()
{
	gpio_sel(pina, OUTPUT);
	async_init(&blinka_pt);
	gpio_sel(pinb, OUTPUT);
	async_init(&blinkb_pt);

	while(1) {
		blinka(&blinka_pt);
		blinkb(&blinkb_pt);
	}
}
