#ifndef __KB_H__
#define __KB_H__
#include "stm32f10x.h"
#include "freertos.h" 
#include "task.h"
#include "stdbool.h"
#include "delay.h"
//g1 g2 g3 g4 g5 g10 g11
#define __READ_C	GPIOG->IDR&(1<<1)
#define __READ_T	GPIOG->IDR&(1<<2)
#define __READ_F	GPIOG->IDR&(1<<3)
#define __READ_L	GPIOG->IDR&(1<<4)
#define __READ_K	GPIOG->IDR&(1<<5)
#define __READ_R	GPIOG->IDR&(1<<10)
#define __READ_B	GPIOG->IDR&(1<<11)
#define __C ispressed(__READ_C)
#define __T ispressed(__READ_T)
#define __F ispressed(__READ_F)
#define __L ispressed(__READ_L)
#define __K ispressed(__READ_K)
#define __R ispressed(__READ_R)
#define __B ispressed(__READ_B)

bool ispressed(uint16_t key);
void kbinit(void);
#endif//__KB_H__

