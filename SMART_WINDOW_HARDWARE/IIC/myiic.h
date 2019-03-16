#ifndef __MYIIC_H
#define __MYIIC_H
#include "delay.h" 
#define IIC_PORT    GPIOC		    												   //IO 所在的组
#define IIC_SCL_BIT 	9															   //管脚号
#define IIC_SDA_BIT 	8															   //管脚号
#define IIC_PORT_CLOCK(opration) 	(RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,opration))//时钟使能
#define IIC_DELAY_US(us) 			delay_us(us)		            					   //延时函数
#define IIC_SDA_MODE_IN  8
#define IIC_SDA_MODE_OUT 3
//IO方向设置
#if (IIC_SDA_BIT>8)
#define IIC_SDA_IN()  {IIC_PORT->CRH &= ~(0XF<<((IIC_SDA_BIT-8)*4));IIC_PORT->CRH|=(IIC_SDA_MODE_IN <<((IIC_SDA_BIT-8)*4));}
#define IIC_SDA_OUT() {IIC_PORT->CRH &= ~(0XF<<((IIC_SDA_BIT-8)*4));IIC_PORT->CRH|=(IIC_SDA_MODE_OUT<<((IIC_SDA_BIT-8)*4));}
#else
#define IIC_SDA_IN()  {IIC_PORT->CRL &= ~(0XF<<((IIC_SDA_BIT-0)*4));IIC_PORT->CRL|=(IIC_SDA_MODE_IN <<((IIC_SDA_BIT-0)*4));}
#define IIC_SDA_OUT() {IIC_PORT->CRL &= ~(0XF<<((IIC_SDA_BIT-0)*4));IIC_PORT->CRL|=(IIC_SDA_MODE_OUT<<((IIC_SDA_BIT-0)*4));}
#endif
//SCL
#define IIC_SCL_L (IIC_PORT->ODR &= ~(1<<IIC_SCL_BIT))
#define IIC_SCL_H (IIC_PORT->ODR |=  (1<<IIC_SCL_BIT))
//SDA	 
#define IIC_SDA_L (IIC_PORT->ODR &= ~(1<<IIC_SDA_BIT))
#define IIC_SDA_H (IIC_PORT->ODR |=  (1<<IIC_SDA_BIT))
#define READ_SDA  ((IIC_PORT->IDR&(1<<IIC_SDA_BIT))>>IIC_SDA_BIT) //输入SDA 
//IIC所有操作函数
void IIC_Init(void);                                                           //初始化IIC的IO口				 
void IIC_Start(void);				                                           //发送IIC开始信号
void IIC_Stop(void);	  			                                           //发送IIC停止信号
void IIC_Send_Byte(u8 txd);			                                           //IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);                                           //IIC读取一个字节
u8 IIC_Wait_Ack(void); 				                                           //IIC等待ACK信号
void IIC_Ack(void);					                                           //IIC发送ACK信号
void IIC_NAck(void);				                                           //IIC不发送ACK信号
#endif
