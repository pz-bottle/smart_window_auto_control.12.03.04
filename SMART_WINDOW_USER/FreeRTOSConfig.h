/*
 * FreeRTOS Kernel V10.0.0
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software. If you wish to use our Amazon
 * FreeRTOS name, please do so in a fair use way that does not cause confusion.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H
#include "stdint.h"
extern uint32_t SystemCoreClock;
/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE. 
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

#define configUSE_PREEMPTION		0//是否使用抢占；实验发现：有可能使得某个任务一直处于就绪态的尴尬境地；\
									 //建议不使用抢占，同时确保每个任务都能腾出时间出去，变成分时操作？？？
#define configUSE_IDLE_HOOK			0
#define configUSE_TICK_HOOK			0 //没有使用的必要
#define configCPU_CLOCK_HZ			( ( uint32_t ) SystemCoreClock )	
#define configTICK_RATE_HZ			( ( TickType_t ) 100 )//100是比较稳定的
#define configMAX_PRIORITIES			( 5 )
#define configMINIMAL_STACK_SIZE		( ( unsigned short ) 128 )
#define configTOTAL_HEAP_SIZE			( ( size_t ) ( 17 * 1024 ) )
#define configMAX_TASK_NAME_LEN			( 16 )//最大的任务名长度
#define configUSE_TRACE_FACILITY		1
#define configUSE_STATS_FORMATTING_FUNCTIONS 	1
#define configUSE_16_BIT_TICKS			0
#define configIDLE_SHOULD_YIELD			1
#define configUSE_TIMERS			1//使用软件定时器
#define configTIMER_TASK_PRIORITY		1//软件定时器任务的优先级
#define configTIMER_QUEUE_LENGTH		5//软件定时器队列长度
#define configTIMER_TASK_STACK_DEPTH		100//软件定时器栈深度
#define configGENERATE_RUN_TIME_STATS		0
#define configCHECK_FOR_STACK_OVERFLOW		1//栈溢出检测
#define configUSE_TASK_NOTIFICATIONS		1//任务通知
//#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS  
//#define portGET_RUN_TIME_COUNTER_VALUE	
/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 			0
#define configMAX_CO_ROUTINE_PRIORITIES 	( 2 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet		1//任务优先级设置
#define INCLUDE_uxTaskPriorityGet		1//任务优先级获取
#define INCLUDE_vTaskDelete			1//任务删除
#define INCLUDE_vTaskCleanUpResources		1//最好包含，否则系统更容易死机（测试发现）
#define INCLUDE_vTaskSuspend			1//任务挂起
#define INCLUDE_vTaskDelayUntil			1//
#define INCLUDE_vTaskDelay			1//延时
#define INCLUDE_xTaskGetSchedulerState		0//获取调度器的状态
#define INCLUDE_uxTaskGetStackHighWaterMark 	1//获取任务剩余的栈空间大小
/* This is the raw value as per the Cortex-M3 NVIC.  Values can be 255
(lowest) to 0 (1?) (highest). */
#define configKERNEL_INTERRUPT_PRIORITY 		255
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	191 /* equivalent to 0xb0, or priority 11. */


/* This is the value being used as per the ST library which permits 16
priority values, 0 to 15.  This must correspond to the
configKERNEL_INTERRUPT_PRIORITY setting.  Here 15 corresponds to the lowest
NVIC value of 255. */
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY	15

/* It is a good idea to define configASSERT() while developing.  configASSERT()
uses the same semantics as the standard C assert() macro. */
extern void vAssertCalled( uint32_t ulLine, const char * const pcFileName );
#define configASSERT( x ) if( ( x ) == 0 ) vAssertCalled( __LINE__, __FILE__ )
//任务切换进入
//#define traceTASK_SWITCHED_IN()


#endif /* FREERTOS_CONFIG_H */

