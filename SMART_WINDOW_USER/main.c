#include "main_task.h"
int main(void)
{
	hardware_init();
	
//	while(1)
//	{
//		if(gsm_call("10086") != 0)
//		{
//			printf("打电话识别\r\n");
//		}else
//		{
//			printf("打电话成功\r\n");
//		}
//		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
//	}
////	while(1)
////	{
////		delay_ms(500);
//////		if(GPIOF->IDR&(1<<9))
//////		{
//////			printf("高电平\r\n");
//////		}else
//////		{
//////			printf("低电平\r\n");
//////		}
////		if(!(GPIOF->IDR&(1<<4)))
////		{
////			printf("低电平\r\n");
////		}else
////		{
////			printf("高电平\r\n");
////		}
////		if(!(GPIOC->IDR&(1<<7)))
////		{
////			printf("7：0\r\n");
////		}else
////		{
////			printf("7:1\r\n");
////		}
////	}
	xTaskCreate( 
		main_task, 
		"main_task",
		1000, 
		NULL,
		0, 
		NULL );
	vTaskStartScheduler();
	return 0;
}
void hardware_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	delay_init();//						--	--
	USART1_Init(115200);//					a9	a10
	gsm_init();//				 		b10	b11	b12
	LED_Init();////						b5
	DHT22Init();///use for dht22 module			f3	--
	LcdInit();/////						D9	E15	E13	E11	E9	 	D0	 						
	kbinit();//////key board				g0	g1 	g2 	g3 	g4 	g5 	g6 	g10	g11
	dust_init();//灰尘模块					a4	a8
	toxicgas_init();//有毒气体模块				a5
	rain_init();//雨滴模块					a2
	Beep_Init();//						b8
	infrared_init();//红外					a0 a3 c6 f4 f5 f7
	voice_init();//录放模块					f2
	anti_steal_init();//磁控防盗初始化			f9
	PCA9685_init(0x40, 50);/*控制舵机控制板  		c8 c9*/
}
void vAssertCalled(uint32_t line,const char *file)
{
	printf("line:%d file:%s",line,file);
	while(1);
}
void assert_failed(uint8_t* file, uint32_t line)
{
	printf("line:%d file:%s",line,file);
	while(1);
}
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
	printf("任务%s发送栈溢出。",pcTaskName);
	while(1);
}
