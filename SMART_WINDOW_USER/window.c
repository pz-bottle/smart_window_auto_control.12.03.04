#include "window.h"
#include "freertos.h"
#include "task.h"
#include <pca9685.h>
#include "dp.h"
extern void set_rainwiper_down_left(uint32_t val);
extern void set_rainwiper_down_right(uint32_t val);
extern void set_rainwiper_stop_left(void);
extern void set_rainwiper_stop_right(void);
//Óê¹Î¸´Î»
void rainwiper_reset(void)
{
	dp("rainwiper_reset£¨£©\r\n");
	uint8_t ok = 0x3;
	if((GPIOC->IDR&(1<<7)))
	{
		set_rainwiper_down_right(1024);
	}
	if((GPIOA->IDR&(1<<0)))
	{	
		set_rainwiper_down_left(1024);
	}
	dp("rainwiper_reset::25\r\n");
	while(ok&0x3)
	{
		if(!(GPIOC->IDR&(1<<7)))
		{
			set_rainwiper_stop_right();
			ok &= ~(1<<0);
		}
		if(!(GPIOA->IDR&(1<<0)))
		{
			set_rainwiper_stop_left();
			ok &= ~(1<<1);
		}
		vTaskDelay(pdMS_TO_TICKS(10));
		dp("rainwiper_reset::%d\r\n", ok);
	}
	dp("rainwiper_reset::26\r\n");
}
/*win1*/
void close_window(void)
{
	PCA9685_setPin(4, 0);
	PCA9685_setPin(5, 2048);
}
void open_window(void)
{
	PCA9685_setPin(4, 2048);
	PCA9685_setPin(5, 0);
}
void stop_window(void)
{
	PCA9685_setPin(4, 0);
	PCA9685_setPin(5, 0);
}
/*win2*/
void close_preventdustwin(int32_t val)
{
	PCA9685_setPin(8, 0);
	PCA9685_setPin(9, 2048);
}
void open_preventdustwin(int32_t val)
{
	PCA9685_setPin(8, 2048);
	PCA9685_setPin(9, 0);
}
void stop_preventdustwin(void)
{
	PCA9685_setPin(8, 0);
	PCA9685_setPin(9, 0);
}


/*C6		F5*/
s8 get_windowstatus(void)
{
	if(!(GPIOC->IDR&(1<<6)))
	{
 
		return WIN_OPEN;
	}
	if(!(GPIOF->IDR&(1<<5)))
	{
 
		return WIN_CLOSE;
	}
	return WIN_UNKNOWN;
}
/*A3		F7*/
s8 get_dustwinstatus(void)
{
	if(!(GPIOA->IDR&(1<<3)))
	{
 
		return DUSTWIN_CLOSE;
	}
	if(!(GPIOF->IDR&(1<<7)))
	{
 
		return DUSTWIN_OPEN;
	}
	return DUSTWIN_UNKNOWN;                    
}
