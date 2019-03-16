#ifndef __READ_TEMPEHUMID_TASK_H
#define __READ_TEMPEHUMID_TASK_H

#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "dp.h"
#include "msgdef.h"
#include "dht22.h"

void read_tempehumid_task(void *perameter);

#endif//__READ_TEMPEHUMID_TASK_H
