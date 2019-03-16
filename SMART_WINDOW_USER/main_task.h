
#include "delay.h"
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "semphr.h" 
#include "stdio.h"
#include "usart.h"
#include "timers.h"
#include "gsm.h"
#include "string.h"
#include "dht22.h"
#include "led.h"
#include "pwm.h"
#include "msgdef.h"
#include "scankey_task.h"
#include "rainwiper_task.h"
#include "gsm.h"
#include "lcd5110.h"
#include "kb.h"
#include "dust.h" 
#include "toxicgas.h"
#include "rain.h"
#include "beep.h"
#include "infrared.h"
#include "window.h"
#include "hc_sr.h"
#include "nrf24l01.h"
#include <pca9685.h>
#include "flash.h"
#include "anti_steal.h"
#include "dp.h"
#include "read_tempehumid_task.h"
#include "flash_alarm_task.h"
#include "watchdog_for_secure_state_task.h"
#include "stealing_task.h"
extern void hardware_init(void);
extern void main_task(void*paremeter);
extern bool is_operatable_screen(void);
extern void timer1_callback(void*parameter);
extern bool is_airpolution(double valug);
static int alarm_tempe = 999;//报警温度 99.9 默认值 程序启动后，在 scankey_task 中会初始化这个变量
extern void set_alarm_tempe(u32 val);
extern bool is_tempeoverpass(int tempe);
extern bool is_toxicgas(u32 val);
extern bool is_rain(int val);
extern void show_tempe_humid_info(u32 tempe,u32 humid);
extern void show_dust_value(double val);
extern bool is_task_suspended(TaskHandle_t xTask);


