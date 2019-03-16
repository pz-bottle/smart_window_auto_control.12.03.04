#include "kb.h"
void kbinit(void)
{//g1 g2 g3 g4 g5 g10 g11
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	GPIO_InitTypeDef  io;
	io.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_10|GPIO_Pin_11;
	
	io.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOG, &io);
	
	//g0
	io.GPIO_Speed = GPIO_Speed_50MHz;
	io.GPIO_Pin = GPIO_Pin_0;
	io.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOG, &io);
	
	//GND PIN
	GPIOG->ODR &= ~(1<<0);
}
bool ispressed(uint16_t key)
{
	key = (!key);
	if(key)
	{
		vTaskDelay(pdMS_TO_TICKS(30)); 
		if(key)	
		{
			vTaskDelay(pdMS_TO_TICKS(180)); 
			return true;
		}
	}
	
	return false;
}

