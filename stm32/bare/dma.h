#pragma once
#include "blue.h"

typedef struct {
	__IO u32 CCR;
	__IO u32 CNDTR;
	__IO u32 CPAR;
	__IO u32 CMAR;
	__IO u32 RESERVED;
} DMA_CHAN_t;


typedef struct
{
	__IO u32 ISR;
	__IO u32 IFCR;
	DMA_CHAN_t CHAN1;
	DMA_CHAN_t CHAN2;
	DMA_CHAN_t CHAN3;
	DMA_CHAN_t CHAN4;
	DMA_CHAN_t CHAN5;
	DMA_CHAN_t CHAN6;
	DMA_CHAN_t CHAN7;
} DMA_t;

//#define GPIO_BASE 0x40010800
#define DMA1 ((DMA_t*) (0x40020000))
#define DMA2 ((DMA_t*) (0x40020400))

#define DMA_CCR_TCIE  (1<<1)
#define DMA_CCR_MINC (1<<7)
#define DMA_CCR_EN (1<<0)
#define DMA_CCR_DIR (1<<4)

#define DMA_ISR_TCIF5 (1<<17)
#define DMA_ISR_TCIF7 (1<<25)

