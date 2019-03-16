
#include "flash_alarm_task.h"
#include "window.h"
u8 alarm = 0;//位0温度报警标志，位1有毒气体报警,为2声光报警标志
TaskHandle_t flash_alarm_task_handle;
void flash_alarm_task(void*parameter)
{
	static int i = 0;
	while(1)
	{
		LED0_TURN;
		BEEP_TURN;
		//开语音报警
		VOICE_ON;
		vTaskDelay(pdMS_TO_TICKS(200));
		printf("flash_alarm_task::语音报警\r\n");
		i++;
		//if(i > 1*30*1000/200)
		if(i > 1*10*1000/200)
		{
			i = 0;
			//关语音报警
			VOICE_OFF;
			LED0_OFF;
			BEEP_OFF;
			//复位声光报警标志
			reset_flash_beep_alarm_flag();
			/*复位入侵条件*/
			reset_invade_flag();
			//挂起自己
			vTaskSuspend( flash_alarm_task_handle );
			
			
			
			/*把窗户关闭*/
			close_window();
			/*等待窗户关闭完成*/
			while(get_windowstatus() != WIN_CLOSE)        
			{
				vTaskDelay(pdMS_TO_TICKS(10));
			}
			stop_window();
			/*稳定一下，防止抖动又触发报警*/
			vTaskDelay(pdMS_TO_TICKS(1000));
		}
	}
}

bool without_beep_alarm_event(void)
{
	bool ok;
	taskENTER_CRITICAL();
	if((alarm&ALARM_MASK) == 0)
		ok = true;
	else 
		ok = false;
	taskEXIT_CRITICAL();
	return ok;
}
void set_flash_beep_alarm_flag(void)
{
	taskENTER_CRITICAL();
	alarm |= FLASH_ALARM_MASK;
	taskEXIT_CRITICAL();
}
void reset_flash_beep_alarm_flag(void)
{
	taskENTER_CRITICAL();
	alarm &= ~FLASH_ALARM_MASK;
	taskEXIT_CRITICAL();
}

void set_toxic_beep_alarm_flag(void)
{
	taskENTER_CRITICAL();
	alarm |= TOXIC_ALARM_MASK;
	taskEXIT_CRITICAL();
}
bool is_toxic_beep_alarm(void)
{
	bool ok = false;
	taskENTER_CRITICAL();
	if((alarm & TOXIC_ALARM_MASK) != 0)
		ok = true;
	taskEXIT_CRITICAL();
	return ok;
}
void set_tempe_beep_alarm_flag(void)
{
	taskENTER_CRITICAL();
	alarm |= TEMPE_ALARM_MASK;
	taskEXIT_CRITICAL();
}
void reset_toxic_beep_alarm_flag(void)
{
	taskENTER_CRITICAL();
	alarm &= ~TOXIC_ALARM_MASK;
	taskEXIT_CRITICAL();
}
void reset_tempe_beep_alarm_flag(void)
{
	taskENTER_CRITICAL();
	alarm &= ~TEMPE_ALARM_MASK;
	taskEXIT_CRITICAL();
}
