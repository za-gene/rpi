#include <vector>

#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#include "abba.h"

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

	prims.insert(prims.end(), pico_prims.begin(), pico_prims.end());
}
