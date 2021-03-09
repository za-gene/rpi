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

enum baby_cmd { BABY_STEP, BABY_FREQ, BABY_INFO, BABY_ON, BABY_OFF};

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
static u32 baby_freqs[8] = { 1,1,1,1,1,1,1,1} ;
bool baby_sound_on = true;

bool baby_timer_step(struct repeating_timer *t)
{
	for(u8 i = 0; i< 8; i++) {
		u8 blue = (i==baby_pos) ? 200 : 0;
		blinkt_set_pixel_colour((i +1) % 8, 0, 0, blue);
	}
	blinkt_show();

	u32 freq = baby_freqs[baby_pos];
	if(!baby_sound_on) freq = 1;
	set_spk_freq(freq);
	
	baby_pos++;
	if(baby_pos == 8) baby_pos = 0;
	
	return true;
}

static u32 baby_timer_setting = 1000;
void set_baby_timer_ms(u32 pulse_ms)
{
	static struct repeating_timer baby_timer;
	cancel_repeating_timer(&baby_timer);
	add_repeating_timer_ms(pulse_ms, baby_timer_step, 0 , &baby_timer);
	baby_timer_setting = pulse_ms;

}

void eval_baby()
{
	int i;
	//printf("eval_baby:reg 0=%d, reg 1=%d, reg2 = %d\n", regs[0], regs[1], regs[2]);
	switch(regs[0]) {
		case BABY_STEP :
			set_baby_timer_ms(regs[1]);
			break;
		case BABY_FREQ:
			if((regs[1] < 1) || (regs[1] > 8)) throw 102;
			i = regs[1]-1;
			printf("eval_baby: set freq of channel %d\n", i);
			baby_freqs[i] = regs[2];
			//set_spk_freq(regs[1]);
			break;
		case BABY_INFO: 
			printf("Pulse rate: %d\n", baby_timer_setting);
			for(i=0; i<8; i++) {
				printf("Channel %d: %d\n", i+1 , baby_freqs[i]);
			}
			break;
		case BABY_ON:
			baby_sound_on = true;
			break;
		case BABY_OFF:
			baby_sound_on = false;
			break;
		default:
			throw 101;
	}

}


void parse_baby()
{
	yylex();
	if(cmd_is("STEP")) {
		embed_num(0, BABY_STEP);
		yylex();
		embed_num(1, yytext);
	} else if(cmd_is("INFO")) {
		embed_num(0, BABY_INFO);
	} else if(cmd_is("ON")) {
		embed_num(0, BABY_ON);
	} else if(cmd_is("OFF")) {
		embed_num(0, BABY_OFF);
	} else {
		embed_num(0, BABY_FREQ);
		embed_num(1, yytext); // the channel (aka step)
		yylex();
		embed_num(2, yytext); // the frequency value
	}

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
