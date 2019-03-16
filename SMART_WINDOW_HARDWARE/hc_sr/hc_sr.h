#ifndef __HC_SR_H__
#define __HC_SR_H__
#include "stm32f10x.h"
#define HC_TRIG_H	(GPIOF->ODR |=  (1<<8))
#define HC_TRIG_L	(GPIOF->ODR &= ~(1<<8))
#define HC_ECHO		(GPIOF->IDR & (1<<9))

void hc_sr04_init(void);
#endif
