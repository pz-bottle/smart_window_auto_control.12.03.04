#ifndef __FLASH_ALARM_TASK_H
#define  __FLASH_ALARM_TASK_H
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "dp.h"
#include "msgdef.h"
#include "led.h"
#include "infrared.h"
#include "beep.h"
#include "stdbool.h"
#include "flash_alarm_task.h"
extern void flash_alarm_task(void*parameter);
extern void reset_tempe_beep_alarm_flag(void);
extern void reset_toxic_beep_alarm_flag(void);
extern void set_tempe_beep_alarm_flag(void);
extern bool is_toxic_beep_alarm(void);
extern void set_toxic_beep_alarm_flag(void);
extern void reset_flash_beep_alarm_flag(void);
extern void set_flash_beep_alarm_flag(void);
extern bool without_beep_alarm_event(void);
extern void reset_invade_flag(void);
extern u8 alarm;//位0温度报警标志，位1有毒气体报警,为2声光报警标志
extern TaskHandle_t flash_alarm_task_handle;
#define ALARM_MASK			(0x7)
#define TEMPE_ALARM_MASK	(0x1<<0)
#define TOXIC_ALARM_MASK	(0x1<<1)
#define FLASH_ALARM_MASK	(0x1<<2)
#endif// __FLASH_ALARM_TASK_H
