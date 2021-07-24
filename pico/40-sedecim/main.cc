/* generates sawtooth wave
*/

#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <tuple>
#include "pico/stdlib.h"
//#include "hardware/adc.h"
//#include "hardware/clocks.h"
//#include "hardware/flash.h"
#include "hardware/gpio.h"
//#include "hardware/irq.h"
#include "hardware/pwm.h"
//#include "hardware/spi.h"
// #include "tusb.h" // if you want to use tud_cdc_connected()

#include "notes.h"
#include "pi.h"
#include "ssd1306.h"
#include "rotary.h"

#define ALARM 	0
#define SPK	15
typedef uint16_t u16;
typedef uint32_t u32;

auto top = 255.0;
auto wave_freq = 440.0;
auto framerate = 44100.0;
auto delay = 1'000'000.0/framerate;
uint slice_num;
float y = 0;
//float dy = top * wave_freq / framerate;
float dy = wave_freq / framerate;


int notes_find(const char* note_name)
{
	for(int i = 0; i<num_notes; i++) {
		if(strcmp(note_name, notes[i].name)==0)
			return i;
	}
	return -1;
}




auto notes_idx = notes_find("C4");


const int num_slots = 16;
int slots[num_slots];
int cur_slot_num = 0;

void set_freq ()
{
	wave_freq = notes[notes_idx].freq;
	dy = wave_freq / framerate;

	// update oled
	char line[128];
	snprintf(line, sizeof(line), "%3.3s %d      ", 
			notes[notes_idx].name, notes[notes_idx].freq);
	setCursorx(0);
	ssd1306_print(line);
	//show_scr();

}

void alarm0_isr()
{
	pi_alarm_rearm(ALARM, delay);
	y += dy;
	if(y>1) y=0;

	// scale the level for the transistor
	//const float low = 0 ? 0.9/3.3*top : 0;
	const float low = 0;
	auto level = low + y* (top-low);
	pwm_set_gpio_level(SPK, level);
}

std::tuple<int, int> cursor_slot(int i) // returns x pos, then y
{
	int x = i<8 ? 0 : 8;
	int y = i<8? i : i -8;
	return std::make_tuple(x, y);
}

#define METRO_ALARM 1
uint64_t metro_delay_us = 1'000'000 * 60 / 120; // 120bpm
#define SCR_GPIO 17 // test of ssd1306 time

void metro_isr()
{
	pi_alarm_rearm(METRO_ALARM, metro_delay_us);
	auto [x, y] = cursor_slot(cur_slot_num);
	ssd1306_print_at(x, y, " ");
	cur_slot_num ++;
	if(cur_slot_num == 16) cur_slot_num = 0;
	std::tie(x, y) = cursor_slot(cur_slot_num);
	ssd1306_print_at(x, y, ">");


}

void rotary_poll(void)
{
	static Rotary rot(21, 20, 19);
	static void* where = &&initialising;
	static int slot_position = 0;
	auto [x, y] = cursor_slot(slot_position);
			//printf("X");
	goto *where;


initialising:
	ssd1306_print_at(x+1, y, "+");
	where = &&navigating;
	//printf("rotary initialising\n");
	return;
navigating: // figuring out which slot to change
	//		printf("-");
	if(int chg = rot.change()) {
		auto new_slot_position = slot_position + chg;
		if(new_slot_position >= num_slots) new_slot_position = 0;
		if(new_slot_position < 0) new_slot_position = num_slots -1;
		if(new_slot_position != slot_position) {
			slot_position = new_slot_position;
			ssd1306_print_at(x+1, y, " ");
			auto [x , y] = cursor_slot(slot_position);
			ssd1306_print_at(x+1, y, "+");
			printf(".");
			//set_freq();

		}

	}
	return;
}

#if 0
void screen_poll()
{
	static bool initialised = false;
	if(!initialised) {
		initialised = true;
		show_scr();
	}
	extern uint8_t scr[1025]; 
	static int page = 0;
	ssd1306_write_cmd(SET_COL_ADDR); // 0x21
	ssd1306_write_cmd(0);
	ssd1306_write_cmd(127);

	ssd1306_write_cmd(SET_PAGE_ADDR); // 0x22
	ssd1306_write_cmd(page);
	ssd1306_write_cmd(page);

	uint8_t mem[1+128];
	mem[0] = 40; // the data instruction
	memcpy(mem+1, scr + 8*page +1, 128);
	gpio_put(SCR_GPIO, true);
	ssd1306_send_data(mem, sizeof(mem));
	page++;
	if(page==8) page=0;
	gpio_put(SCR_GPIO, false);
}
#endif

int main() 
{
	stdio_init_all();
	// while(!tud_cdc_connected()) sleep_ms(250); // wait for usb serial 

#define BTN  14 // GPIO number, not physical pin
#define LED  25 // GPIO of built-in LED
	gpio_init(BTN);
	gpio_set_dir(BTN, GPIO_IN);
	gpio_pull_up(BTN);
	// gpio_get() gets state of pin

	pi_gpio_init(SCR_GPIO, OUTPUT);

	gpio_init(LED);
	gpio_set_dir(LED, GPIO_OUT);

	gpio_set_function(SPK, GPIO_FUNC_PWM);
	slice_num = pwm_gpio_to_slice_num(SPK);
	pwm_set_wrap(slice_num, top);
	pwm_set_enabled(slice_num, true); // let's go!
	pi_alarm_init(ALARM, alarm0_isr, delay);
	pi_alarm_init(METRO_ALARM, metro_isr, metro_delay_us);

	init_display(64, 4); // ssd1036

	//init slots:
	for(int i =0; i< num_slots; i++) {
		slots[i] = notes_idx; // to middle C
		auto [x, y] = cursor_slot(i);
		ssd1306_print_at(x+2, y, notes[notes_idx].name);
	}

	for(;;) {
		rotary_poll();
		ssd1306_display_cell();
	}

	return 0;
}

