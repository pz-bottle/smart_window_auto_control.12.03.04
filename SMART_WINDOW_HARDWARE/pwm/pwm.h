#ifndef __PWM_H__
#define __PWM_H__
#include "stm32f10x.h"
void pwm_init(void);
void set_angle(u8 angle,s8 channel,TIM_TypeDef* TIMx);
#endif//__PWM_H__
