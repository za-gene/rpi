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
        TIM4->PSC=7200;
        TIM4->ARR=9999;
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

Assuming a clock frequency of 72MHz, I use a prescaler of 7200, and ARR (Auto-Reload Register) of 9999 for a counter clock frequency
CK_CNT - f_CK_PSC/(PSC[15:0]+1) so the frequency is 72MHz/7200/(9999+1), which is 1Hz. So my thinking is a little wrong somewhere.

The secs count of 50 took about 45secs on clock using a delay of 1000.

