#include "anti_steal.h"
#include "stm32f10x.h"
/*初始化化F9用于防入室盗窃功能*/
void anti_steal_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				      
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;/*上拉*/ 		//MODIFY BY YPL 改为下拉 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOF, &GPIO_InitStructure);						
}
bool anti_steal_is_danger_signal(void)
{
	return (GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_9) == Bit_SET);/*磁控开关输出高电平代表触发？*/  
}

