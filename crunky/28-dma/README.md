# Memory-to-memory DMA

Trying to get my head around the whole deal.


## Speed test

Transfer of 6 bytes using a naive memcpy (optimisation disabled) took 5us. Using DMA took 3us.

Transfer of 1MB took 349ms using memcpy, but only 12.8ms using DMA. That's a speedup of 27X. 

I then turned on O3 optimsation. Transfer of 1MB now took only 27ms, compared to the previous 349ms. 


## Links

* [forum](https://www.raspberrypi.org/forums/viewtopic.php?f=72&t=317883&e=1&view=unread#unread)



## Status

2021-08-19	Working
