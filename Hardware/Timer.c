#include "stm32f10x.h"                  // Device header

void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_ClockDivision =  TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode =  TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period  =  1000-1;//0.01秒
	TIM_TimeBaseInitStruct.TIM_Prescaler = 720-1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel =  TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd  = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority  = 3;
	NVIC_Init(&NVIC_InitStruct);

	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	TIM_Cmd(TIM3,ENABLE);
}


