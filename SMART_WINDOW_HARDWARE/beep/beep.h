#ifndef __BEEP_H__
#define __BEEP_H__
#include "stm32f10x.h"
#define BEEP_ON		GPIOB->ODR |= (1<<8)
#define BEEP_OFF	GPIOB->ODR &= ~(1<<8)
#define BEEP_TURN	GPIOB->ODR ^= (1<<8)
void Beep_Init(void);
#endif//__BEEP_H__
