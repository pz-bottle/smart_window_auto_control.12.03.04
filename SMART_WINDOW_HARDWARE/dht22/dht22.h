#ifndef __DHT22_H__
#define __DHT22_H__
#include "stm32f10x.h"
//#include "timer_delay.h"
#include "delay.h"
#include "freertos.h"
/********************************************************************************************************
 * 无需修改.c文件，只需修改.h文件即可
 *******************************************************************************************************/
typedef unsigned char u8;
#define DHT22_PORT    GPIOF	//stm32的哪一组端口	    												   //IO 所在的组
#define DHT22_DQ_BIT 	3	//这组端口的哪一位														   //管脚号
#define DHT22_PORT_CLOCK(opration) 	(RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,opration))//io时钟使能
#define DHT22_DELAY_MS(ms) 			delay_ms(ms)	            					   //延时函数
#define DHT22_DELAY_US(us) 			delay_us(us)	            					   //延时函数
#define DHT22_DQ_MODE_IN  8//不要改
#define DHT22_DQ_MODE_OUT 3//不要改
//IO方向设置
#if (DHT22_DQ_BIT>8)
#define DHT22_DQ_IN()  do{DHT22_PORT->CRH &= ~(0XF<<((DHT22_DQ_BIT-8)*4));DHT22_PORT->CRH|=(DHT22_DQ_MODE_IN <<((DHT22_DQ_BIT-8)*4));}while(0)
#define DHT22_DQ_OUT() do{DHT22_PORT->CRH &= ~(0XF<<((DHT22_DQ_BIT-8)*4));DHT22_PORT->CRH|=(DHT22_DQ_MODE_OUT<<((DHT22_DQ_BIT-8)*4));}while(0)
#else
#define DHT22_DQ_IN()  do{DHT22_PORT->CRL &= ~(0XF<<((DHT22_DQ_BIT-0)*4));DHT22_PORT->CRL|=(DHT22_DQ_MODE_IN <<((DHT22_DQ_BIT-0)*4));}while(0)
#define DHT22_DQ_OUT() do{DHT22_PORT->CRL &= ~(0XF<<((DHT22_DQ_BIT-0)*4));DHT22_PORT->CRL|=(DHT22_DQ_MODE_OUT<<((DHT22_DQ_BIT-0)*4));}while(0)
#endif
//DQ 
#define DHT22_DQ_L (DHT22_PORT->ODR &= ~(1<<DHT22_DQ_BIT))
#define DHT22_DQ_H (DHT22_PORT->ODR |=  (1<<DHT22_DQ_BIT))
#define READ_DQ  ((DHT22_PORT->IDR&(1<<DHT22_DQ_BIT))>>DHT22_DQ_BIT) //输入 


void DHT22Init(void);

u8 DHT22ReadTempeHumid(int *temp,int *humi);
 
#endif//__DHT22_H__
