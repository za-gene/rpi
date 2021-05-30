# Random

See if we can generate random noise. Output is on GP19. Using GP20, it's possible to toggle
between using `random()` and the hardware to generate white noise.

To my surprise, using `random()` is faster than
generating a byte using `rosc_hw->randombit`:
```
Generating 10,000 hardware bytes ... took 11862 us
Generating 10,000 random()s ... took 5773 us
```

## Status

2021-05-30 Sampling freq 40kHz

2021-03-17 started.

## Links

* [forum](https://www.raspberrypi.org/forums/viewtopic.php?f=145&t=307225&sid=27b40fa2b27ff592a24a4e2fe5a23897)

