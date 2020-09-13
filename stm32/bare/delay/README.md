# clock


The critical bits are
```
void delay(uint16_t ms)
{
        TIM4->CNT =0;
        while(TIM4->CNT < ms);
}


void main()
{
       // timer setup
        RCC_APB1ENR |= RCC_APB1ENR_TIM4EN;
        TIM4->PSC=7999;
        TIM4->ARR=10000;
        TIM4->CR1 |= TIM_CR1_CEN;

	// looping
	int secs = 0;
	while ( 1 ) {
		itoa(secs++, life, 10);
		puts2(life);
		delay(1000);
	}
}
```

The system clock frequency is 8MHz. Setting PSC to 7999 therefore increments CNT with a frequency of
```
CK_CNT = f_CK_PSC/(PSC[15:0]+1) = 8,000,000/(7,999+1) = 1kHz
```

SO `CNT` is updated every 1ms, and is reset when it reaches the ARR (Auto-Reload Register) value.

Note that `PSC`, `CNT` and `ARR` have 16-bit values (0-65,535) even though they are 32-bit registers.

Timing reveals that the `delay(1000)` produced the expected delay of 1s. It seems that the other processing in the loop did not throw the calcs out.
