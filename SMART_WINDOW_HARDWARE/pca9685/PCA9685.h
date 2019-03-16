#ifndef __PCA9685_H
#define __PCA9685_H

#include <stm32f10x.h>
#include <stdbool.h>
#include <myiic.h>
#include <math.h>

#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4

#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD
static uint8_t _i2caddr;
uint8_t PCA9685_read8(uint8_t addr);
void PCA9685_write8(uint8_t addr, uint8_t d);
void PCA9685_reset(void);
void PCA9685_setPWMFreq(float freq);
void PCA9685_setPWM(uint8_t num, uint16_t on, uint16_t off);
void PCA9685_setPin(uint8_t num, uint16_t val);
void PCA9685_init(uint8_t dev_addr, float  pwmfreq);
void PCA9685_set_pulsh_ms(uint8_t num, float val);
#endif//__PCA9685_H

