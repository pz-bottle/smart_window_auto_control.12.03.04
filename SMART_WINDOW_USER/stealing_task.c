#include "stealing_task.h"
#include "flash_alarm_task.h"
static bool invade_flag = false;
bool get_invade_flag()/* watchdog_for_secure_state_task 和flash_task中调用*/
{
	taskENTER_CRITICAL();
	bool ok =  invade_flag;
	taskEXIT_CRITICAL();
	return ok;
}
void set_invade_flag()/*stealing_task中设置*/
{
	taskENTER_CRITICAL();
	invade_flag = true;
	taskEXIT_CRITICAL();
}
void reset_invade_flag()/*flash_alarm_task中复位*/
{
	taskENTER_CRITICAL();
	invade_flag = false;
	taskEXIT_CRITICAL();
}
TaskHandle_t stealing_task_handle;

/*磁控开关防盗任务 */
void stealing_task(void *p)
{
	dp("stealing_task start");
	int8_t phone[12]={0};
	/*循环判断是否有人入侵*/
	while(1)
	{
		/*让出CPU 10毫秒*/
		vTaskDelay(pdMS_TO_TICKS(10));
		dp("stealing_task running...\r\n");
		/*判断是否发生了入室盗窃动作*/
		if(is_invade_cond()) /*键盘锁了（屏幕没有锁）且检测到磁控开关的响应信号,且不是自己在开、关窗户*/           
		{
			dp("设置一下标志位\r\n");
			set_invade_flag();/*设置一下标志，让其他人知道有人入侵了*/
			dp("stealing_task::is_invade_cond\r\n");
			/*如果声光报警任务处于休眠状态*/
			if(is_task_suspended(flash_alarm_task_handle))
			{
				/*唤醒声光报警任务*/
				vTaskResume( flash_alarm_task_handle);
				taskENTER_CRITICAL();
				{
					flash_read(FLAHS_SAVE_ALARM_PHOME_ADDR, (uint8_t*)phone, 11);    
					//snprintf((char*)phone, 11, "10086 ");
					dp("打电话报警:%s，有人入侵\r\n", phone);
					if(gsm_call(phone) != 0)
					{
						if(gsm_call(phone) != 0)
						{
							if(gsm_call(phone) != 0)
							{
								printf("电话报警失败, 尝试短信报警\r\n");
								if(gsm_send_message((u8*)"Your home is dangerous.", (u8*)phone) != 0)
								{
									if(gsm_send_message((u8*)"Your home is dangerous.", (u8*)phone) != 0)
									{
										if(gsm_send_message((u8*)"Your home is dangerous.", (u8*)phone) != 0)
										{
											dp("报警失败\r\n");
										}else
										{
											printf("报警成功\r\n");
										}
									}		
								}								
							}else
							{
								printf("电话报警成功\r\n");
							}			
						}else
						{
							printf("电话报警成功\r\n");
						}			
					}else
					{
						printf("电话报警成功\r\n");
					}						
				}					
				taskEXIT_CRITICAL();
			}
		}else
		{
			
		}
	}
	/*绝对不能运行到这里，如果一定要，必须在此处删除这个任务，否则CPU就跑飞了*/
}
extern bool get_windows_self_operation_flag(void);
/*判断是否有人入侵，非法打开窗户*/
bool is_invade_cond(void)
{
	return false;
	/*	怎么判断是合法还是非法操作？：键盘未上锁时，是合法的，否则是非法的	
		如果用户解锁了键盘，但是忘记上锁后就出门去了，这时候有人入室盗窃怎
		么处理？键盘解锁后，智能窗处于非安全状态，这种状态下不会触发破窗而
		入的报警动作；因此需要利用看门狗的思想：定时喂狗，否则狗叫（上锁键
		盘，恢复到安全状态。那如何定时喂狗？在键盘任务中的每一个while循环
		中都加一个喂狗操作，且每次喂狗的时间小于等于定时喂狗时间即可！这里
		可以利于操作系统提供的任务通知功能来解决,
		watchdog_for_secure_state_task方法用于监测智能窗是否长时间处
		于非安全状态,feed_dog_for_secure_state方法用于喂狗）
	*/
	
	/*判断磁控开发发生响应动作*/
	if(anti_steal_is_danger_signal())
	{
		
		if(keyboard_is_locked())/*如果键盘是上锁的*/
		{
			/*进一步判断是不是自己打开窗户，如果下雨关窗、发现有毒其他开窗的操作不能认为有人入侵了*/
			if(get_windows_self_operation_flag())
			{
				return false;
			}else
			{
				return true;
			}
		}else
		{
			return false;/*键盘没有上锁，那就是认为开关窗户的操作*/
		}
	}else
	{
		return false;
	}
}
