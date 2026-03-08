#include "stm32f10x.h"                  // Device header

uint16_t AD_Value[3];
void AD_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin  =  GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed  =  GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_IPU;     //普通IO口不能配置成AIN
	GPIO_InitStruct.GPIO_Pin  =  GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed  =  GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //配置AD的分频，只能6，8分频
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_28Cycles5);//规则组配置
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_28Cycles5);
	
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_ContinuousConvMode =  ENABLE;     //连续模式
	ADC_InitStruct.ADC_DataAlign =  ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv  = 	ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_Mode  =  ADC_Mode_Independent;  //单双ADC
	ADC_InitStruct.ADC_NbrOfChannel =  2;      //菜单的数目
	ADC_InitStruct.ADC_ScanConvMode =  ENABLE;  //扫描模式
	ADC_Init(ADC1,&ADC_InitStruct);
	
	DMA_InitTypeDef DMA_InitStruct;
	DMA_InitStruct.DMA_BufferSize =  2;//计数器
	DMA_InitStruct.DMA_DIR   = DMA_DIR_PeripheralSRC; //传输方向   外设作为源
	DMA_InitStruct.DMA_M2M  =  DMA_M2M_Disable;//是否存储器到存储器
	DMA_InitStruct.DMA_Mode  =   DMA_Mode_Circular;//是否开启自动重装
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)AD_Value;
	DMA_InitStruct.DMA_MemoryDataSize  =  DMA_MemoryDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryInc  = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralBaseAddr  = (uint32_t)&ADC1->DR;
	DMA_InitStruct.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_PeripheralInc =  DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_Priority  = DMA_Priority_High;
    DMA_Init(DMA1_Channel1,&DMA_InitStruct);
	
	DMA_Cmd(DMA1_Channel1,ENABLE);
	ADC_Cmd(ADC1,ENABLE);
	ADC_DMACmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1)==SET);
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1)==SET);
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}
//void ADC_GetValue(void)
//{
//	DMA_Cmd(DMA1_Channel1,DISABLE);
//	DMA_SetCurrDataCounter(DMA1_Channel1,2);
//	DMA_Cmd(DMA1_Channel1,ENABLE);
//	
//	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
//	while(DMA_GetFlagStatus(DMA1_FLAG_TC1)==RESET);
//	DMA_ClearFlag(DMA1_FLAG_TC1);
//	
//	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==RESET)
//		AD_Value[2] = 1;
//	else  AD_Value[2] = 0;
//} 
void ANJIAN_Get(void)
{
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==SET)
		AD_Value[2] = 0;
	else  AD_Value[2] = 1;
}
	