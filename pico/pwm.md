# PWM

## Calculate divider from freq and top

```
float pwm_divider(int freq, int top)
{
        uint32_t f_sys = clock_get_hz(clk_sys); // typically 125'000'000 Hz
        float scale = (top+1) * freq;
        return f_sys / scale;
}
```

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
