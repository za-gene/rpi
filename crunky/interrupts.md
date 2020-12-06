# Interrupts

According to [Timer interrupt example](https://www.raspberrypi.org/forums/viewtopic.php?f=72&t=286072),
interrupts are enabled on aarch32 architectures as follows:

```
cpsie i ; enable interrupts
cpsie f; enable fast interrupts (FIQ)
```

On aarch64:

```
msr daifclr, #2; enable interrupts
msr daifclr, #1; enable fast interrupts (FIQ)
```

It further states: "in both cases the enabling instruction should be followed by an instruction sync barrier assembly instruction", like so:

```
isb
```

As of 2020-12-06, `enable_irq()` is implemented in `vectors.S` by the assembly code:

```
.globl enable_irq
enable_irq:
    mrs r0,cpsr
    bic r0,r0,#0x80
    msr cpsr_c,r0
    bx lr
```

