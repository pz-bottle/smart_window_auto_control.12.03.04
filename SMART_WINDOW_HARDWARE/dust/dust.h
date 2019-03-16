#ifndef __DUST_H__
#define __DUST_H__
#include "stm32f10x.h"
#include "delay.h"

#define DUST_ADC_CH 4

#define LED_H	(GPIOA->ODR |=  (1<<8))

#define LED_L	(GPIOA->ODR &= ~(1<<8))

void dust_init(void);

u32 dust_get_advalue(void );
	
double dust_get(uint32_t val);

#endif//__DUST_H__
