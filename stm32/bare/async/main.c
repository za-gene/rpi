#include <stdbool.h>

#include <async.h>
#include <blue.h>
#include <gpio.h>
#include <timers.h>


#define LED1 PB12
#define LED2 PB13

u32 get_system_timer()
{
	u16 lo = RTC_CNTL;
	u16 hi = RTC_CNTH;
	u32 res = hi;
	res <<= 16;
	res |= lo;
	return res;
}

typedef struct {uint64_t start; uint64_t duration;} timer_t;

timer_t timer1, timer2;

void start_timer(timer_t* t, int duration)
{
	t->start = TIM4->CNT; // this is in milliseconds
	t->duration = duration; // in millis
}

bool timer_expired(timer_t* t)
{
	const int scale = 1;
	return (TIM4->CNT - t->start) > t->duration * scale;
}




static async blink1(struct async *pt)
{
	async_begin(pt);
	while(1) {
		start_timer(&timer1, 10);
		gpio_write(LED1, 1);
		await(timer_expired(&timer1));

		start_timer(&timer1, 900);
		gpio_write(LED1, 0);
		await(timer_expired(&timer1));
	}
	async_end;
}

static async blink2(struct async *pt)
{
        async_begin(pt);
        while(1) {
                start_timer(&timer2, 500);
                gpio_write(LED2, 1);
                await(timer_expired(&timer2));

                start_timer(&timer2, 500);
                gpio_write(LED2, 0);
                await(timer_expired(&timer2));
        }
        async_end;
}



static struct async blink1_pt, blink2_pt;


void main()
{
	// init timer
	RCC_APB1ENR |= RCC_APB1ENR_TIM4EN;
	TIM4->PSC=7999;
	TIM4->ARR=65535;
	TIM4->CR1 |= TIM_CR1_CEN;
	//
	gpio_mode_out(LED1);
	//gpio_write(LED1, 1);

	gpio_mode_out(LED2);
	gpio_write(LED2, 1);
	//RCC_APB1ENR |= (1<<28); // PWREN interface clock

	async_init(&blink1_pt);
	async_init(&blink2_pt);
	u32 start;
	while (1) {
		blink1(&blink1_pt);
		blink2(&blink2_pt);
		continue;
		gpio_write(LED1, 1);
		start_timer(&timer1, 100);
		while(!timer_expired(&timer1));

		gpio_write(LED1, 0);
		start_timer(&timer1, 1000);
		while(!timer_expired(&timer1));
	}

	/*
	   {
	   u16 tmp = val++;
	   for(u8 i=1; i<=8; i++) {
	   u16 digit = tmp %10;
	   tmp = tmp / 10;
	   transfer_7219(i,digit);
	   }
	   delayish(200);
	   }
	   */
}

