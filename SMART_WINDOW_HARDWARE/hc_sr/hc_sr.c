#include "hc_sr.h"

void hc_sr04_init(void)
{
	 GPIO_InitTypeDef  io;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	 io.GPIO_Pin = GPIO_Pin_8;				      
	 io.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	 io.GPIO_Speed = GPIO_Speed_50MHz;		
	 GPIO_Init(GPIOF, &io);					
	 io.GPIO_Pin = GPIO_Pin_9;				      
	 io.GPIO_Mode = GPIO_Mode_IPD; 		
	 GPIO_Init(GPIOF, &io);				
	 GPIOF->ODR &= ~(1<<7);
}


