#ifndef __RAINWIPER_TASK__
#define __RAINWIPER_TASK__
#include "freertos.h"
#include "task.h"
#include <stdio.h>
#include "pwm.h"
#include "delay.h"
#include "pca9685.h"
#include "dp.h"
static TaskHandle_t rainwiper_task_handle;
void rainwiper_task(void *parameter);
extern void setrainwiper_break(int ms);
extern void rainwiper_reset(void);

void open_window(void);
void close_window(void);
#endif//__RAINWIPER_TASK__
