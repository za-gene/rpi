# RC low-pass filter

Implements a low-pass RC filter with cut-off frequency 400Hz. 

Put a speaker between GP19 and ground.

Optionally, attach a pushbutton switch to GP20. Normally, filtered noise
is output, but when you push down the button, the unfiltered white noise
is output.

It's a variant on rcfilter-1, which uses pwm instead of just random bits.

## Status

2021-06-10 started. seems to work


## Links

* [blog](https://mcturra2000.wordpress.com/2021/06/02/sleepico-a-pleasant-noise-generator-for-the-raspberrypi-rp2040/) post
* db05.296 for equation derivation



