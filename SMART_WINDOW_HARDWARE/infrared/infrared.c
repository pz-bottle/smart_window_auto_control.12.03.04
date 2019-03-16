#include "infrared.h"
#include "delay.h"
#include "dp.h"
#include "window.h"
void infrared_init(void)
{
	/*a0 a3 c6 c7 f4 f5 f7*/
	GPIO_InitTypeDef io;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOF, ENABLE);	
	
	//以下是红外探头的初始化
	io.GPIO_Speed = GPIO_Speed_2MHz;
	io.GPIO_Mode = GPIO_Mode_IPU;//上拉输入 
	io.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;
	GPIO_Init(GPIOF,&io);
	
	io.GPIO_Mode = GPIO_Mode_IPU;//上拉输入 
	io.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init(GPIOC, &io);
	
	io.GPIO_Mode = GPIO_Mode_IPU;//上拉输入 
	io.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3;
	GPIO_Init(GPIOA, &io);
}
void voice_init(void)
{
	GPIO_InitTypeDef io;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	
	io.GPIO_Mode = GPIO_Mode_Out_PP;
	io.GPIO_Pin = GPIO_Pin_2;
	io.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOF,&io);
	VOICE_OFF;
	VOICE_OFF;
}
