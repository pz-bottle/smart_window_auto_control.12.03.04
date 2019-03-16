#ifndef __STEALING_TASK_H
#define __STEALING_TASK_H
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "dp.h"
#include "msgdef.h"
#include "led.h"
#include "infrared.h"
#include "beep.h"
#include "stdbool.h"
#include "window.h"
#include "scankey_task.h"
#include "anti_steal.h"
extern bool is_invade_cond(void);
extern void feed_dog_for_secure_state(void);
extern void feed_dog_for_secure_state(void);
extern bool is_task_suspended(TaskHandle_t xTask);
extern bool is_operatable_screen(void);
extern TaskHandle_t stealing_task_handle;
extern void stealing_task(void *p);
extern void set_flag(int i);
extern int get_flag();
#endif//__STEALING_TASK_H
