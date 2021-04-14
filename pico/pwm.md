# PWM

## Calculate divider from freq and top

```
/** NB clock divider must be in range 1.f <= value < 256.f
*/

float pwm_divider(int freq, int top)
{
        uint32_t f_sys = clock_get_hz(clk_sys); // typically 125'000'000 Hz
        float scale = (top+1) * freq;
        return f_sys / scale;
}
```
The above function is defined in `pace.c`.

Use it in something like this:

```
        uint slice_num = pwm_gpio_to_slice_num(SPK);

        int top = 255;
        int sampling_freq = 44'100;
        sampling_freq = 800;
        float divider = pwm_divider(sampling_freq, top);
        pwm_set_clkdiv(slice_num, divider); // pwm clock should now be running at 1MHz

        gpio_set_function(SPK, GPIO_FUNC_PWM);
        pwm_set_wrap(slice_num, top);
        pwm_set_enabled(slice_num, true);
        pwm_set_gpio_level(SPK, top/2);
```

## Using an IRQ

File `pace.h` gives a way of configuring a PWM with an IRQ. Example:

```
#include "pace.h"

#define SPK 19

unsigned int slice_num;

void my_pwm_isr()
{
        // do stuff like:
        pwm_set_gpio_level(SPK, new_level);
 
        pwm_clear_irq(slice_num); // reset the irq so that it will fire in future
}

int main()
{
        stdio_init_all();
        int err = pace_config_pwm_irq(&slice_num, SPK, sampling_freq, top, my_pwm_isr);
        if(err) {
            puts("Can't set PWM");
            for(;;); // loop forever, effectively halting
        }

        ...
}
```

A fully compileable example is available in [sine440-pwm](sine440-pwm).



