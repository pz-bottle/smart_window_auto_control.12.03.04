#include "watchdog_for_secure_state_task.h"
extern bool get_invade_flag();
extern TaskHandle_t scankey_task_handle;
TaskHandle_t watchdog_for_secure_state_task_handle;
void watchdog_for_secure_state_task(void *p)
{
	dp("watchdog_for_secure_state_task start");
	/* The rate at which the periodic task generates software interrupts. */
	const TickType_t xMaxExpectedBlockTime = pdMS_TO_TICKS( 8000UL );
	uint32_t ulEventsToProcess;
	eTaskState task_state;
	/*等扫描键盘任务创建成功后才继续执行*/
	while((eTaskGetState( scankey_task_handle ) != eReady))
	{
		dp("等扫描键盘任务创建成功后才继续执行.\r\n");
		vTaskDelay(pdMS_TO_TICKS(100));
	}
	while(1)
	{	
		ulEventsToProcess = ulTaskNotifyTake( pdTRUE, xMaxExpectedBlockTime );
		if( ulEventsToProcess == 0 )
		{
			if(is_operatable_screen()==false)/*键盘处于解锁状态*/
			{
				if(get_invade_flag()==false)/*没有入侵条件发生*/
				{
					dp("到了\r\n");
					dp("kill scankey_task");
					/*	在这里将智能窗恢复到安全状态	
						首先杀死扫描键盘任务，然后在
						重新创建键盘任务
					*/
					vTaskDelete( scankey_task_handle );
					/*确保任务正确删除了*/
					task_state = eTaskGetState(  scankey_task_handle );
					if( (task_state != eReady		) && \
						(task_state != eBlocked		) && \
						(task_state != eSuspended	) && \
						(task_state != eRunning		)	 \
					  )
					{
						dp("删除任务成功\r\n");
//						if(get_windowstatus() != WIN_CLOSE)
//						{
//							/*把窗户关闭*/
//							close_window();
//							/*等待窗户关闭完成*/
//							while(get_windowstatus() != WIN_CLOSE)        
//							{
//								vTaskDelay(pdMS_TO_TICKS(10));
//							}
//							stop_window();
//						}
						//创建扫描键盘任务
						xTaskCreate( 
							scankey_task, 
							"scankey_task",
							500, 
							NULL,
							1, 
							NULL );
						dp("创建scankey_task成功\r\n");
					}else
					{
						dp("删除任务失败\r\n");
					}
				}
			}
		}
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}
void feed_dog_for_secure_state(void)
{
	/*对通知值加一操作*/
	xTaskNotifyGive( watchdog_for_secure_state_task_handle);
}
