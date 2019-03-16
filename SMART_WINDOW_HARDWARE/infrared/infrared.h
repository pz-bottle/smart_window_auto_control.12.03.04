#ifndef __INFRARED_H__
#define __INFRARED_H__
#include "stm32f10x.h"
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "msgdef.h"
void infrared_init(void);
void voice_init(void);
#define VOICE_OFF	GPIOF->ODR &= ~(1<<2)
#define VOICE_ON	do{GPIOF->ODR &= ~(1<<2);GPIOF->ODR |= (1<<2);}while(0)
#endif//__INFRARED_H__
