#include "rainwiper_task.h"

static uint32_t pca9685_pwm_val = 1024;//雨刷最快的间隔

void setrainwiper_break(int pwm_val)
{
	taskENTER_CRITICAL();
	if(pca9685_pwm_val < 1024) pca9685_pwm_val = 1024;
	else if(pwm_val > 2048)    pca9685_pwm_val = 2048;
	else               		   pca9685_pwm_val = pwm_val;
	taskEXIT_CRITICAL();
}

/*设置左边雨刷的电机*/
void set_rainwiper_up_left(uint32_t val)
{
	PCA9685_setPin(10, 2048);
	PCA9685_setPin(11, 0);
}
/*设置左边雨刷的电机*/
void set_rainwiper_down_left(uint32_t val)
{
	PCA9685_setPin(11, 2048);
	PCA9685_setPin(10, 0);
}
void set_rainwiper_stop_left(void)
{
	PCA9685_setPin(10, 0);
	PCA9685_setPin(11, 0);
}

/*设置右边雨刷的电机*/
void set_rainwiper_up_right(uint32_t val)
{
	PCA9685_setPin(6, 2048);
	PCA9685_setPin(7, 0);
}
/*设置右边雨刷的电机*/
void set_rainwiper_down_right(uint32_t val)
{
	PCA9685_setPin(6, 0);
	PCA9685_setPin(7, 2048);
}
void set_rainwiper_stop_right(void)
{
	PCA9685_setPin(6, 0);
	PCA9685_setPin(7, 0);
}

/*上a0 下f6 ；；； 下f4 上c7*/
void rainwiper_task(void *parameter)
{
	dp("rainwiper_task start");
	uint8_t dir_left = 0;
	uint8_t dir_right = 0;
	while(1)
	{
		if(!(GPIOA->IDR&(1<<0)))
		{
			//10 shang 11 xiao
			//上
			dir_left = 1;
//			taskENTER_CRITICAL();
//			set_rainwiper_stop_right(0);
//			taskEXIT_CRITICAL();
//			vTaskDelay(pdMS_TO_TICKS(500));
		}
		if(!(GPIOF->IDR&(1<<6)))
		{
			//下
			dir_left = 0;
//			taskENTER_CRITICAL();
//			set_rainwiper_stop_right(0);
//			taskEXIT_CRITICAL();
//			vTaskDelay(pdMS_TO_TICKS(500));
		}
		if(!(GPIOC->IDR&(1<<7)))
		{
			//10 shang 11 xiao
			//上
			dir_right = 1;
//			taskENTER_CRITICAL();
//			set_rainwiper_stop_right(0);
//			taskEXIT_CRITICAL();
//		、言	vTaskDelay(pdMS_TO_TICKS(500));
		}
		if(!(GPIOF->IDR&(1<<4)))
		{
			//下
			dir_right = 0;
//			taskENTER_CRITICAL();
//			set_rainwiper_stop_right(0);
//			taskEXIT_CRITICAL();
//			vTaskDelay(pdMS_TO_TICKS(500));
		}
		if(dir_right)
		{
			//printf("UP\r\n");
			set_rainwiper_up_right(pca9685_pwm_val);
		}
		else
		{
			//printf("down\r\n");
			set_rainwiper_down_right(pca9685_pwm_val);
		}
		
		
		if(dir_left)
		{
			set_rainwiper_up_left(pca9685_pwm_val);	 
		}
		else
		{
			set_rainwiper_down_left(pca9685_pwm_val);
			 
		}
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}
