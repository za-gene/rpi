#include <vector>

#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
//#include "hardware/timer.h"
#include "pico/stdlib.h"

#include "abba.h"
#include "../blinkt.h"

using namespace std;

extern vector<prim_t> prims;

/*
   typedef struct spkr {
   spkr();
   } spkr_t;



   spkr_t speaker;
   */


#define SPK 14

void set_spk_freq(u32 f_pwm)
{

	u16 duty = 50;
	uint slice_num = pwm_gpio_to_slice_num(SPK);

	// set frequency
	// determine top given Hz - assumes free-running counter rather than phase-correct
	u32 f_sys = clock_get_hz(clk_sys); // typically 125'000'000 Hz
	float divider = f_sys / 1'000'000UL;  // let's arbitrarily choose to run pwm clock at 1MHz
	pwm_set_clkdiv(slice_num, divider); // pwm clock should now be running at 1MHz
	u32 top =  1'000'000UL/f_pwm -1; // TOP is u16 has a max of 65535, being 65536 cycles
	pwm_set_wrap(slice_num, top);

	// set duty cycle
	u16 level = (top+1) * duty / 100 -1; // calculate channel level from given duty cycle in %
	pwm_set_chan_level(slice_num, 0, level);
}

static u8 baby_pos = 0;

bool baby_timer_step(struct repeating_timer *t)
{
	for(u8 i = 0; i< 8; i++) {
		if(i == baby_pos)
			blinkt_set_pixel_colour(i, 0, 0, 10);
		else
			blinkt_set_pixel_colour(i, 0, 0, 0);
	}
	blinkt_show();

	baby_pos++;
	if(baby_pos == 8) baby_pos = 0;
	
	return true;
}

void set_baby_timer_ms(u32 pulse_ms)
{
	static struct repeating_timer baby_timer;
	cancel_repeating_timer(&baby_timer);
	add_repeating_timer_ms(pulse_ms, baby_timer_step, 0 , &baby_timer);

}

void eval_baby()
{
	u32 f_pwm = regs[0];
	//f_pwm = 440;
	set_spk_freq(f_pwm);

}


void parse_baby()
{
	yylex();
	push_bcode(Load(0, xstoi(yytext)));
	push_bcode(Call(eval_baby));
}

vector<prim_t> pico_prims =
{
	{"BABY", parse_baby, eval_baby}
};

void init_abba_pico()
{
	puts("init_abba_pico:called");
	gpio_set_function(SPK, GPIO_FUNC_PWM); 
	uint slice_num = pwm_gpio_to_slice_num(SPK);
	pwm_set_enabled(slice_num, true);


	set_baby_timer_ms(1000);
	blinkt_init(16, 17);

	prims.insert(prims.end(), pico_prims.begin(), pico_prims.end());
}
