#ifndef LED_H_
#define LED_H_
#include "stm32f10x.h"
void LED_Init(void);

#define LED0_OFF  GPIO_WriteBit(GPIOB,  GPIO_Pin_5, Bit_SET )
#define LED0_ON   GPIO_WriteBit(GPIOB,  GPIO_Pin_5, Bit_RESET )
#define LED1_OFF  GPIO_WriteBit(GPIOE,  GPIO_Pin_5, Bit_SET )
#define LED1_ON   GPIO_WriteBit(GPIOE,  GPIO_Pin_5, Bit_RESET )
#define LED0_TURN GPIOB->ODR^=1<<5
#define LED1_TURN GPIOE->ODR^=1<<5
#endif//LED_H_
