#include "stm32f10x.h"  	// Device header
#include "Delay.h"

uint8_t Key_Num;
void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}
uint8_t Key_GetNum(void)
{
	uint8_t tem = 0;
	if(Key_Num)
	{
		tem = Key_Num;
	    Key_Num = 0;
		return tem;
	}
	return 0;
}
uint8_t KeyGetNum(void)           
{
	uint8_t KeyNum = 0;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0)KeyNum = 1;		
	return KeyNum;
}
void Key_Tick(void)
{
	static uint8_t Key_val,Key_old;
	static uint32_t Key_slow = 0;
	if(++Key_slow<2)return;
	Key_slow = 0;
	Key_val =  KeyGetNum();
	if(Key_val==0&&Key_old!=0) Key_Num = Key_old; //按下检测
//	if(Key_val!=0&&Key_old==0) Key_Num = Key_old; //松手检测
	Key_old = Key_val;
}
