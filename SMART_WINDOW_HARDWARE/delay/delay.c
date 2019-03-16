//
#include "delay.h"
/**************************************************************
 * 函数名称：delay_init
 * 功能描述：配置定时器用作延时函数
 * 输入参数：定时器时钟总线频率（MHz）
 * 输出参数：无
 * 返回值  ：无
 **************************************************************/
static u8 __SYSCLK_MHz = 72;//延时函数初始化后获得系统时钟大小，单位MHz
extern uint32_t SystemCoreClock;
void delay_init(void)
{
	__SYSCLK_MHz = SystemCoreClock/1000000;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1, ENABLE );
	TIM_TimeBaseStructInit( &TIM_TimeBaseStructure );
	TIM_TimeBaseStructure.TIM_Period = 100;//默认计数值
	TIM_TimeBaseStructure.TIM_Prescaler = __SYSCLK_MHz;//分频到微妙级
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;//向下计数，配合延时程序使用
	TIM_TimeBaseInit( TIM1, &TIM_TimeBaseStructure );
	TIM_Cmd( TIM1, ENABLE );
}
/**************************************************************
 * 函数名称：delay_us
 * 功能描述：微妙延时函数
 * 输出参数：延时微妙数
 * 输出参数：无
 * 返回值  ：无
 **************************************************************/
void delay_us(unsigned int us)
{
	if(us <= 0) return;
	TIM1->PSC = __SYSCLK_MHz;
	TIM1->ARR = us;
	TIM1->CNT = us;
	while(TIM1->CNT);//注意计数器工作于向下计数模式
}
/**************************************************************
 * 函数名称：delay_ms
 * 功能描述：毫秒延时函数
 * 输出参数：延时毫妙数
 * 输出参数：无
 * 返回值  ：无
 **************************************************************/
void delay_ms(unsigned int ms)
{	
	TIM1->PSC = __SYSCLK_MHz;
	TIM1->ARR = 1000;
	while(ms--)
	{	
		TIM1->CNT = 1000;
		while(TIM1->CNT);//注意计数器工作于向下计数模式
	}
}

