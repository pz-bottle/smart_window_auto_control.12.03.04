#include "main_task.h"

void main_task(void*paremeter)
{
	//队列
	xQueueHandle queue_handle;
	TimerHandle_t timer;
	BaseType_t ok;
	timer =xTimerCreate("timer1_callback",
		pdMS_TO_TICKS(600) ,//period
		pdTRUE,		    //atuo reload 
		NULL,
		timer1_callback ); 
	if(timer != NULL)
		xTimerStart(timer,pdMS_TO_TICKS(100));

	//创建一个队列用来任务间的通信
	queue_handle = xQueueCreate(100,sizeof(struct message));
	configASSERT(queue_handle != NULL);
	#if 1
	//创建读取温度湿度任务
	ok = xTaskCreate( 
		read_tempehumid_task, //任务函数
		"read_tempehumid_task",
		500, //栈深度 
		queue_handle,
		1, 
		NULL );
	configASSERT(ok == pdPASS);
	#endif
	printf("read_tempehumid_task ok\r\n");
	//创建扫描键盘任务
	ok = xTaskCreate( 
		scankey_task, 
		"scankey_task",
		500, 
		NULL,
		1, 
		&scankey_task_handle );
	configASSERT(ok == pdPASS); 
	printf("scankey_task ok\r\n");
	//创建安全状态看门狗任务
	ok = xTaskCreate( 
		watchdog_for_secure_state_task, 
		"watchdog_for_secure_state_task",
		128, 
		NULL,
		1, 
		&watchdog_for_secure_state_task_handle );
	configASSERT(ok == pdPASS);
	printf("watchdog_for_secure_state_task ok\r\n");
	#if 1
	//创建防盗任务
	ok = xTaskCreate( 
		stealing_task, 
		"stealing_task",
		200, 
		NULL,
		1, 
		&stealing_task_handle );
	configASSERT(ok == pdPASS);
	printf("stealing_task_handle ok\r\n");
	#endif
	//创建雨刮任务
	ok = xTaskCreate( 
		rainwiper_task, 
		"rainwiper_task",
		200, 
		NULL,
		1, 
		&rainwiper_task_handle );
	configASSERT(ok == pdPASS);
	vTaskSuspend( rainwiper_task_handle );
	printf("rainwiper_task_handle ok\r\n");
	//声光报警
	ok = xTaskCreate( 
		flash_alarm_task, 
		"flash_alarm_task",
		100, 
		NULL,
		1, 
		&flash_alarm_task_handle );
	configASSERT(ok == pdPASS);
	vTaskSuspend( flash_alarm_task_handle );
	printf("flash_alarm_task_handle ok\r\n");
	printf("all task ok\r\n");
	
	while(1)
	{
		vTaskDelay(pdMS_TO_TICKS(10));
#if 1
		dp("main task.\r\n");
		static u32 toxicval;
		toxicval= toxicgas_getad(5);//adc1 a5
		if(is_toxicgas(toxicval) == true)//发现有毒气体
		{
			set_flag(1);
			dp("发现有毒气体");
			dp("有毒气体值%d",toxicval);
			//鸣笛报警
			BEEP_ON;
			set_toxic_beep_alarm_flag();//有毒气体警报标志 
			//判断玻璃窗打开了没有
			if(get_windowstatus() != WIN_OPEN)
			{
				//打开窗户
				open_window();
			}
			/*等待打开窗户成功*/
			while(get_windowstatus() != WIN_OPEN)
			{
				vTaskDelay(pdMS_TO_TICKS(10));
			}
			/*停止电机*/
			stop_window();
			
			
			//窗纱没有打开
			if(get_dustwinstatus() != DUSTWIN_OPEN)
			{
				//打开纱窗
				open_preventdustwin(0);
				dp("关闭防雾窗纱中...\n");
			}
			//等待判断打开了没有
			while(get_dustwinstatus() != DUSTWIN_OPEN)
			{
				vTaskDelay(pdMS_TO_TICKS(10));
			}
			/*停止电机*/
			stop_preventdustwin();
			set_flag(0);
			
		}else
		{
			reset_toxic_beep_alarm_flag();//无有毒气体
			if(without_beep_alarm_event() == true)//无任何警情
			{
				BEEP_OFF;
			}
		}
		static double dustad;
		taskENTER_CRITICAL();
		{
			dustad = dust_get_advalue();
		}
		taskEXIT_CRITICAL();
		dp("dustval%lf\r\n",dustad);
		static double dustug;
		dustug = dust_get(dustad)*1000;
		static int dustovertime = 0;
		if(is_airpolution(dustug) == true)//发现空气污浊
			dustovertime++;
		else
			dustovertime = 0;
		/*dustovertime的意思是说连续的发现颗粒物浓度超标时就认为是超标了*/
		if(dustovertime > 50)//发现空气污浊
		{
			dp("发现空气污浊");
			//窗纱没有关
			if(get_dustwinstatus() != DUSTWIN_CLOSE)
			{
				//关防雾霾窗纱
				close_preventdustwin(0);
				dp("关闭防雾窗纱中...\n");
			}
			//等待判断关闭完成了没有
			while(get_dustwinstatus() != DUSTWIN_CLOSE)
			{
				vTaskDelay(pdMS_TO_TICKS(10));
			}
			/*停止电机*/
			stop_preventdustwin();
		}       
		static struct message msg;
		static struct dht22msg thmsg;
		if(xQueueReceive(queue_handle,&msg,0) == pdPASS)//类似与linux中的poll机制
		{
			switch(msg.type)
			{
			case E_DHT22:// 温湿度信息
	
				thmsg = *(struct dht22msg*)msg.info; 
					
				if(is_operatable_screen())//屏幕可写
				{
					dp("屏幕可写");
					show_tempe_humid_info(thmsg.tempe,thmsg.humid);
					show_dust_value(dustug);
				}else
				{
					dp("屏幕不可写");
				}
				if((is_tempeoverpass(thmsg.tempe) == true))//温度超过设定值
				{
					dp("温度报警...");
					set_tempe_beep_alarm_flag();
					BEEP_ON;
				}else
				{
					reset_tempe_beep_alarm_flag();//温度报警位取消
					//温度恢复正常
					if(without_beep_alarm_event() == true)//无有毒气体和温度报警
					{
						dp("无警情");
						BEEP_OFF;	
					}
				}
			break;
			}
		}	
		static int32_t rain_val;
		rain_val = rain_getad(2);//adc1 通道2
		printf("雨量值%d\r\n",rain_val);
		if((is_rain( rain_val) == true) && (is_toxic_beep_alarm()==false))//下雨了 且没有有毒气体
		{
			set_flag(1);
			dp("下雨了");
			dp("雨量值%d",  rain_val);
			/*关闭窗户*/
			if(get_windowstatus() != WIN_CLOSE)
			{
				//关闭窗户
				close_window();
			}
			//等待判断关闭完成了没有
			while(get_windowstatus() != WIN_CLOSE)
			{
				printf("等待窗户关闭成功\r\n");
				vTaskDelay(pdMS_TO_TICKS(10));
			}
			/*停止电机*/
			stop_window();
			
			
			if(is_task_suspended(rainwiper_task_handle) == true)
			{
				dp("启动雨刮任务");
				//启动雨刮任务
				vTaskResume( rainwiper_task_handle  );
			}
			dp("根据雨量的大小设置雨刮的频率");
			//根据雨量的大小设置雨刮的频率
			setrainwiper_break(rain_val);
			dp("设置雨刮频率为%d\r\n",rain_val);
		}else 
		{ 
			if(is_task_suspended(rainwiper_task_handle) != true)
			{
				dp("挂起雨刮任务，关闭雨刷并复位");
				
				//挂起雨刮任务，关闭雨刷并复位
				vTaskSuspend( rainwiper_task_handle );
				//成功挂起，然后复位
				if(is_task_suspended(rainwiper_task_handle) == true)
				{
					dp("挂起雨刮任务，关闭雨刷并复位"); 
					rainwiper_reset();
					dp("复位成功\r\n");
				}
					set_flag(0);
			}
		}
		
	#endif
	}
}
void timer1_callback(void*parameter)
{
	dp("timer1_callback start");
	static s8 f;
	f++;
	taskENTER_CRITICAL();
	{
		if(f&1)
			LcdWriteString(0,0," SmartWindow   ");
		else
			LcdWriteString(0,0,"             ");
	}
	taskEXIT_CRITICAL();
	LED0_TURN;                                 
}
bool is_airpolution(double valug)
{
	return (valug >= 75);//
} 
void set_alarm_tempe(u32 val)
{
	taskENTER_CRITICAL();
	alarm_tempe = val;
	taskEXIT_CRITICAL();
}
bool is_tempeoverpass(int tempe)
{
	return (tempe > alarm_tempe);
}
bool is_toxicgas(u32 val)
{
	return (val > 1024);	//1024/4096量程的1/4，灵敏度
}
bool is_rain(int val)
{
	return (val < 1000);//有雨时低电平输出的
}
void show_tempe_humid_info(u32 tempe,u32 humid)
{
	char str[20];
	taskENTER_CRITICAL();
	{
		LcdWriteString(0,4,"                 "); 	
		sprintf(str,"tempe:%d.%d     ",tempe/10,tempe%10);
		LcdWriteString(0,1,(u8*)str); 
		sprintf(str,"humid:%d.%d     ",humid/10,humid%10);
		LcdWriteString(0,2,(u8*)str); 
	}
	taskEXIT_CRITICAL();
}
void show_dust_value(double val)
{
	taskENTER_CRITICAL();
	{
		char str[20];
		sprintf(str,"dustv:%.0lf       ",val);
		LcdWriteString(0,3,(u8*)str);
	}
	taskEXIT_CRITICAL();
}
bool is_task_suspended(TaskHandle_t xTask)
{
	return (eTaskGetState( xTask ) == eSuspended);
}



