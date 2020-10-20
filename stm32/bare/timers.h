#pragma once

// section 15.4.18 TIMx register map page 423
typedef struct {
	__IO uint32_t CR1; // 0x00
	__IO uint32_t CR2; // 0x04
	__IO uint32_t SMCR; // 0x08
	__IO uint32_t DIER; // 0x0C
	__IO uint32_t SR; // 0x10
	__IO uint32_t EGR; // 0x14
	__IO uint32_t CCMR1; // 0x18
	__IO uint32_t CCMR2; // 0x1C
	__IO uint32_t CCER; // 0x20
	__IO uint32_t CNT; // 0x24
	__IO uint32_t PSC; // 0x28
	__IO uint32_t ARR; // 0x2C
	__IO u32 reserved1; // 0x30
	__IO u32 CCR1; // 0x34
	__IO u32 CCR2; // 0x38
	__IO u32 CCR3; // 0x3C
	__IO u32 CCR4; // 0x40
	__IO u32 reserved2; // 0x44
	__IO u32 DCR; // 0x49
	__IO u32 DMAR; // 0x4C
} TIMx_t;


#define TIM4	((TIMx_t*) 0x40000800)
#define TIM3	((TIMx_t*) 0x40000400)
#define TIM2	((TIMx_t*) 0x40000000)

#define TIM_CR1_CEN (1<<0)


// section 15.4.4 TIMx DMA/Interrupt enable register (TIMx_DIER)
#define TIM_DIER_UIE (1<<0)
#define TIM_DIER_TIE (1<<6)

#define TIM_EGR_UG (1<<0)
#define TIM_EGR_TG (1<<6)

#define TIM_CCER_CC1E (1<<0)

#define TIM_SR1_UIF (1<<0)
//#define TIM_DIER_UIE (1<<0)
#define TIM_CCMR1_OC1PE (1<<3)
