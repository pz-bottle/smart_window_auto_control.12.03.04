#ifndef __WATCHDOG_FOR_SECURE_STATE_TASK_H
#define __WATCHDOG_FOR_SECURE_STATE_TASK_H
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "dp.h"
#include "window.h"
#include "scankey_task.h"
#include "flash_alarm_task.h"
extern TaskHandle_t scankey_task_handle;
extern TaskHandle_t watchdog_for_secure_state_task_handle;
extern bool keyboard_is_locked(void);
extern void watchdog_for_secure_state_task(void *p);
#endif//__WATCHDOG_FOR_SECURE_STATE_TASK_H
