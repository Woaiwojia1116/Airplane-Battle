#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "OLED_Data.h"
#include "eye.h"
#include "Timer.h"
#include <time.h>
#include <stdlib.h>

uint16_t time_rand;
int main(void)
{
	srand(time_rand);
	OLED_Init();
	Timer_Init();
    Game_Init();
	while (1)
	{
		Eyes_Show();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
	}
}
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
	{
		Eyes_Tick();
		time_rand++;
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
	
}

