#include "read_tempehumid_task.h"
void read_tempehumid_task(void *perameter)
{
	dp("read_tempehumid_task start");
	xQueueHandle queue_handle = (xQueueHandle)perameter;
	struct dht22msg msg;
	struct message _msg;
	int ok;
	_msg.type = E_DHT22;
	while(1)
	{
		taskENTER_CRITICAL();
		{
			ok = DHT22ReadTempeHumid(&(msg.tempe),&(msg.humid));
		}
		taskEXIT_CRITICAL();
		if(ok == 0)
		{	
			_msg.info = (void*)(&msg);
			//将温度湿度数据发送出去
			if(xQueueSendToBack(queue_handle,&_msg,100) == pdPASS)
			{
				dp("温度湿度消息发送成功.");
			}
		}else
		{
			printf("读取温度失败");
		}
		vTaskDelay(pdMS_TO_TICKS(500));
		dp("温湿度任务运行中...");
	}
}

