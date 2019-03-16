#include "stm32f10x.h"
#include "pwm.h"
#include "delay.h"
#include "dp.h"
void pwm_init(void)
{
	GPIO_InitTypeDef PWM_IO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);
	
	PWM_IO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//复用功能
	PWM_IO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	PWM_IO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, & PWM_IO_InitStruct);
		
	PWM_IO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_Init( GPIOC, & PWM_IO_InitStruct);
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,  ENABLE);//重映射
	
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;//向下计数模式
	TIM_TimeBaseInitStruct.TIM_Period = 0xFFFF;//周期设定到最大
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72-1;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//不设这个值还出不来pwm波，因为会断言失败
	
	TIM_TimeBaseInit( TIM2, & TIM_TimeBaseInitStruct);
	TIM_TimeBaseInit( TIM3, & TIM_TimeBaseInitStruct);

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;//配合向下计数
	
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OCInitStruct.TIM_OutputState = ENABLE;
	TIM_OCInitStruct.TIM_OutputNState = DISABLE;
	/*TIM_SetCompare1( TIM2, 1100+900);
		TIM_SetCompare2( TIM2, 550);
		
		TIM_SetCompare3( TIM2, 520);
		TIM_SetCompare4( TIM2,  500+900);
	 */
	TIM_OCInitStruct.TIM_Pulse = 2000;//基准脉冲
	TIM_OC1Init( TIM2, & TIM_OCInitStruct);//a0
	TIM_OCInitStruct.TIM_Pulse = 550;//基准脉冲
	TIM_OC2Init( TIM2, & TIM_OCInitStruct);//a1
	TIM_OCInitStruct.TIM_Pulse = 520;//基准脉冲
	TIM_OC3Init( TIM2, & TIM_OCInitStruct);//a2
	TIM_OCInitStruct.TIM_Pulse = 500+900;//基准脉冲
	TIM_OC4Init( TIM2, & TIM_OCInitStruct);//a3
	
	TIM_OCInitStruct.TIM_Pulse = 860;//基准脉冲
	TIM_OC1Init( TIM3, & TIM_OCInitStruct);//c6
	TIM_OCInitStruct.TIM_Pulse = 1800;//基准脉冲
	TIM_OC2Init( TIM3, & TIM_OCInitStruct);//c7

	TIM_OC3Init( TIM3, & TIM_OCInitStruct);//c8
	TIM_OC4Init( TIM3, & TIM_OCInitStruct);//c9
	
	TIM_Cmd( TIM2, ENABLE);
	TIM_Cmd( TIM3, ENABLE);
	delay_ms(1000);
	dp("众神归位");
	delay_ms(1000);
	TIM_Cmd( TIM2, DISABLE);
	TIM_Cmd( TIM3, DISABLE);
	
	////////////////////// 以上的这个意思是设定周期很大pwm波使得舵机缓慢归位避免一下子归位弄坏齿轮/////////////////////////////////////////////////
	/////////////接下去才是真正要设置的值//////////////////////////////////////////
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;//向下计数模式
	TIM_TimeBaseInitStruct.TIM_Period = 10000-1;//对于控制舵机，设大一些，对速度要求没那么高，太小也不太好，容易损坏器件，也不易控制
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72-1;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//不设这个值还出不来pwm波，因为会断言失败
	
	TIM_TimeBaseInit( TIM2, & TIM_TimeBaseInitStruct);
	TIM_TimeBaseInit( TIM3, & TIM_TimeBaseInitStruct);

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;
	
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OCInitStruct.TIM_OutputState = ENABLE;
	TIM_OCInitStruct.TIM_OutputNState = DISABLE;
	/*
	TIM_SetCompare1( TIM2, 1100+900);
		TIM_SetCompare2( TIM2, 550);
		
		TIM_SetCompare3( TIM2, 520);
		TIM_SetCompare4( TIM2,  500+900);
		
		TIM_SetCompare1( TIM2, 1100+900);
		TIM_SetCompare2( TIM2, 550);
		
		TIM_SetCompare3( TIM2, 520);
		TIM_SetCompare4( TIM2,  500+900);
	
	*/
	TIM_OCInitStruct.TIM_Pulse = 1100+900;//基准脉冲
	TIM_OC1Init( TIM2, & TIM_OCInitStruct);//a0
	TIM_OCInitStruct.TIM_Pulse = 550;//基准脉冲
	TIM_OC2Init( TIM2, & TIM_OCInitStruct);//a1
	TIM_OCInitStruct.TIM_Pulse = 520;//基准脉冲
	TIM_OC3Init( TIM2, & TIM_OCInitStruct);//a2
	TIM_OCInitStruct.TIM_Pulse = 500+900;//基准脉冲
	TIM_OC4Init( TIM2, & TIM_OCInitStruct);//a3
	
	TIM_OCInitStruct.TIM_Pulse = 860;//这个通道设定的初始脉冲
	TIM_OC1Init( TIM3, & TIM_OCInitStruct);//c6	
	TIM_OCInitStruct.TIM_Pulse = 1800;//这个通道设定的初始脉冲
	TIM_OC2Init( TIM3, & TIM_OCInitStruct);//c7
	TIM_OCInitStruct.TIM_Pulse = 0;//基准脉冲 
	TIM_OC3Init( TIM3, & TIM_OCInitStruct);//c8
	TIM_OC4Init( TIM3, & TIM_OCInitStruct);//c9
	
	TIM_Cmd( TIM2, ENABLE);
	TIM_Cmd( TIM3, ENABLE);
}



