#ifndef __SCANKEY_TASK_H__
#define __SCANKEY_TASK_H__
#include <stdbool.h>
#include <stdio.h>
#include "freertos.h"
#include "task.h"
#include "stm32f10x.h"
#include "kb.h"
#include "lcd5110.h"
#include "window.h"
#include "gsm.h"
#include "dp.h"
#include "led.h"
#include "beep.h"
#include "flash.h"
void scankey_task(void *parameter);
extern TaskHandle_t scankey_task_handle;
#endif//__SCANKEY_TASK_H__
