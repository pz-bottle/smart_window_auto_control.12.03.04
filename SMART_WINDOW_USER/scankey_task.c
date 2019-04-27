#include "scankey_task.h"
#define delay_while() vTaskDelay(pdMS_TO_TICKS(10))//在while循环中加入插入延时防止一直占用CPU
static volatile bool keyboard_not_locked = false;//屏幕是否已经上锁
static s8 ui_menuindex = 0;
static s8 ui_setphoneindex = 1;/*一开始就指向电话号码的第1位，例如：13570574320，执向3*/
static s8 ui_settempeindex = 0;
//static s8 ui_windowopsindex = 0;
static  u8 alarmtempe[5] = {""},tmpalarmtempe[5]={"12.3"};//最终这个数保存到stm32 flash
static  u8 alarmphone[12] = {""},tmpalarmphone[12]={"13570574320"};//最终这个数保存到stm32 flash
bool is_operatable_screen(void)//除了键盘程序其他进程只能获取屏幕是否可操作状态后可以写才能写，这个函数提供这个接口
{
	bool ret;
	taskENTER_CRITICAL();
	ret = keyboard_not_locked;
	taskEXIT_CRITICAL();
	return (ret == false);
}
void get_alarm_phone(char *phone)
{
	strncpy(phone,(char*)alarmphone,12);
}
//主菜单
void ui_menu(void)																								
{
	taskENTER_CRITICAL();
	LcdWriteString(0,1,"  "); 
	LcdWriteString(0,2,"  "); 
	LcdWriteString(0,3,"  "); 
	s8 i = ui_menuindex;
	LcdWriteString(0,i+1,">"); 
	LcdWriteString(6,1,"alarm phone    "); 
	LcdWriteString(6,2,"alarm tempe    "); 
	LcdWriteString(6,3,"window1 ops    "); 
	LcdWriteString(6,4,"window2 ops    "); 
	taskEXIT_CRITICAL();
}
//光标前移
void ui_menuprev(void)
{
	s8 i = ui_menuindex;
	taskENTER_CRITICAL();
	LcdWriteString(0,i+1," "); 
	
	i = (i-1+4)%4;
	
	LcdWriteString(0,i+1,">"); 
	taskEXIT_CRITICAL();
	ui_menuindex = i;
}
//光标后移
void ui_menunext(void)
{
	s8 i = ui_menuindex;
	taskENTER_CRITICAL();
	LcdWriteString(0,i+1," "); 
	i = (i+1+4)%4;
	LcdWriteString(0,i+1,">");
	taskEXIT_CRITICAL();
	ui_menuindex = i;
}
//设置报警号码
void ui_setphone(void)
{
	s8 i = ui_setphoneindex;
	taskENTER_CRITICAL();
	{
		LcdWriteString(0,4,"             ");
		LcdWriteString(0,3,"             ");
		LcdWriteString(0,2,"             ");
		LcdWriteString(0,1,"             "); 
		
		LcdWriteString((i+1)*6,2,"^");  
		LcdWriteString(6,1,tmpalarmphone); 
	}
	taskEXIT_CRITICAL();
}
//光标左移
void ui_setphoneprev(void)
{
	s8 i = ui_setphoneindex;
	taskENTER_CRITICAL();
	LcdWriteString((i+1)*6,2," ");
	
	i = (i-1+11)%11;
	if(i == 0)
		i = (i-1+11)%11;
	LcdWriteString((i+1)*6,2,"^");  
	taskEXIT_CRITICAL();
	ui_setphoneindex = i;
}
//光标右移
void ui_setphonenext(void)
{
	s8 i = ui_setphoneindex;
	taskENTER_CRITICAL();
	LcdWriteString((i+1)*6,2," ");

	i = (i+1+11)%11;
	if(i == 0)
		i = (i+1+11)%11;
	LcdWriteString((i+1)*6,2,"^");  
	taskEXIT_CRITICAL();
	ui_setphoneindex = i;
}
//加一
void ui_setphoneup(void)
{
	s8 i = ui_setphoneindex;
	tmpalarmphone[i] = (tmpalarmphone[i]-'0'+1+10)%10 + '0';
	taskENTER_CRITICAL();
	LcdWriteString(6,1,tmpalarmphone);
	taskEXIT_CRITICAL();
}
//减一
void ui_setphonedown(void)
{
	s8 i = ui_setphoneindex;
	tmpalarmphone[i] = (tmpalarmphone[i]-'0'-1+10)%10 + '0';
	taskENTER_CRITICAL();
	LcdWriteString(6,1,tmpalarmphone); 
	taskEXIT_CRITICAL();
}
//更新
void ui_updatephone(void)
{
	taskENTER_CRITICAL();
	LcdWriteString(6,1,"updating...");
	taskEXIT_CRITICAL();
}
//设置报警温度
void ui_settempe(void)
{
	s8 i = ui_settempeindex;
	taskENTER_CRITICAL();
	{
		LcdWriteString(0,4,"              ");
		LcdWriteString(0,3,"              ");
		LcdWriteString(0,2,"              ");
		LcdWriteString(0,1,"              ");
		
		LcdWriteString((i+1)*6,2,"^");
		LcdWriteString(6,1,(u8*)tmpalarmtempe);
	}
	taskEXIT_CRITICAL();
}
//光标左移
void ui_settempeprev(void)
{
	s8 i = ui_settempeindex;
	taskENTER_CRITICAL();
	{
		LcdWriteString((i+1)*6,2," ");
		i = (i-1+4)%4;
		if(i == 2)//跳过小数点
			i = (i-1+4)%4;
		LcdWriteString((i+1)*6,2,"^");
		printf("ui_settempeprev::%d\r\n", i);
	}
	taskEXIT_CRITICAL();	
	ui_settempeindex = i;
	
}
//光标右移
void ui_settempenext(void)
{
	s8 i = ui_settempeindex;
	taskENTER_CRITICAL();
	{
		LcdWriteString((i+1)*6,2,(u8*)" ");
		i = (i+1+4)%4;
		if(i == 2)//跳过小数点
			i = (i+1+4)%4;
		LcdWriteString((i+1)*6,2,(u8*)"^");
		//printf("ui_settempenext::%d\r\n", i);
	}
	taskEXIT_CRITICAL();
	ui_settempeindex = i;
	
}
//加一
void ui_settempeup(void)
{
	s8 i = ui_settempeindex;
	tmpalarmtempe[i] = (tmpalarmtempe[i]-'0'+1+10)%10 + '0';
	taskENTER_CRITICAL();
	{
		LcdWriteString(6,1,(u8*)tmpalarmtempe);
		//printf("%s\r\n", tmpalarmtempe);
	}
	taskEXIT_CRITICAL();
}
//减一
void ui_settempedown(void)
{
	s8 i = ui_settempeindex;
	tmpalarmtempe[i] = (tmpalarmtempe[i]-'0'-1+10)%10 + '0';
	taskENTER_CRITICAL();
	{
		LcdWriteString(6,1,(u8*)tmpalarmtempe);
		//printf("%s\r\n", tmpalarmtempe);
	}
	taskEXIT_CRITICAL();
	
}
//更新
void ui_updatetempe(void)
{
	taskENTER_CRITICAL();
	LcdWriteString(6,1,(u8*)"updating...");
	taskEXIT_CRITICAL();
}
void ui_windowops(void)
{
	taskENTER_CRITICAL();
	LcdWriteString(0,4,"                 ");  
	LcdWriteString(0,3,"                 ");  
	LcdWriteString(0,1,"  <window1>   ");  
	LcdWriteString(0,2,"<close> <open>");  
	LcdWriteString(0,3," KEY L OR R   ");  
	
	taskEXIT_CRITICAL();
}
//光标前指
void ui_windowopsprev(void)
{
	taskENTER_CRITICAL();
	LcdWriteString(0,3,"  <<<=======");
	taskEXIT_CRITICAL();
}
//光标后指
void ui_windowopsnext(void)
{
	taskENTER_CRITICAL();
	LcdWriteString(0,3,"  =======>>>");
	taskEXIT_CRITICAL();
}
void ui_dustwinops(void)
{
	taskENTER_CRITICAL();
	
	LcdWriteString(0,1,"  <window2>   ");  
	LcdWriteString(0,2,"<close> <open>");  
	LcdWriteString(0,3," KEY L OR R   ");  
	LcdWriteString(0,4,"                ");  
	taskEXIT_CRITICAL();
}
void ui_tip_user(u8 x,u8 y,u8 tip[])
{
	taskENTER_CRITICAL();
	LcdWriteString(x,y,tip);  	
	taskEXIT_CRITICAL();
}

//光标前指
void ui_dustwinopsprev(void)
{
	taskENTER_CRITICAL();
	LcdWriteString(0,3,"  <<<=======");
	taskEXIT_CRITICAL();
}
//光标后指
void ui_dustwinopsnext(void)
{
	taskENTER_CRITICAL();
	LcdWriteString(0,3,"  =======>>>");
	taskEXIT_CRITICAL();
}
bool is_gotosetphone(void)
{
	return (ui_menuindex == 0);
}
bool is_gotosettempe(void) 
{
	return (ui_menuindex == 1);
}
bool is_gotowindowops(void)
{
	return (ui_menuindex == 2);
}
bool is_gotodustwinops(void) 
{
	return (ui_menuindex == 3);
}
extern void feed_dog_for_secure_state(void);
extern u32 set_alarm_tempe(u32 val);
#define update_alarm_phone_tempe()		     do{taskENTER_CRITICAL();\
							flash_write(FLAHS_SAVE_ALARM_PHOME_ADDR, (u8*)tmpalarmphone, 12);\
							flash_write(FLASH_SAVE_ALARM_TEMP_ADDR,  (u8*)tmpalarmtempe, 5);\
							taskEXIT_CRITICAL();}while(0) /*gsm_set_namecard(tmpalarmphone,tmpalarmtempe,1)*/
#define get_alarm_phone_tempe(alarmphone,alarmtempe) do{taskENTER_CRITICAL();\
							flash_read (FLAHS_SAVE_ALARM_PHOME_ADDR, (u8*)tmpalarmphone, 12);\
							flash_read (FLASH_SAVE_ALARM_TEMP_ADDR,  (u8*)tmpalarmtempe, 5);\
							taskEXIT_CRITICAL();}while(0) /*gsm_get_namecard(alarmphone,alarmtempe,1)*/
static bool scankey_task_had_init = false;
extern void watchdog_for_secure_state_task(void *a);
extern TaskHandle_t watchdog_for_secure_state_task_handle;
TaskHandle_t scankey_task_handle;														   
void scankey_task(void *parameter)
{
//	update_alarm_phone_tempe();
	LcdReset();
	LcdClear();
	dp("scankey_task start");
	float tmp;
	dp("准备喂狗");
	/*等扫描键盘任务创建成功后才继续执行*/
	// while((eTaskGetState( watchdog_for_secure_state_task ) != eReady))
	while((eTaskGetState( watchdog_for_secure_state_task_handle ) != eReady))
	{
		dp("watchdog_for_secure_state_task not ok.\r\n");
		vTaskDelay(pdMS_TO_TICKS(10));
	}
	dp("ready");
	feed_dog_for_secure_state();
	dp("喂狗成功");
	#if 1
	if(scankey_task_had_init == false)/*如果没有初始化过，初始化一下*/
	{
		scankey_task_had_init = true;/*记一下已经初始化过了*/	
		/*	读取保存在flash中的值,报警的电话号码	*/
		flash_read(FLAHS_SAVE_ALARM_PHOME_ADDR, (u8*)alarmphone,12);
		/*	读取保存在flash中的值,报警的温度	*/
		flash_read(FLASH_SAVE_ALARM_TEMP_ADDR, (u8*)alarmtempe, 5);       
		/*	扫描到变量中保存	*/
		sscanf((char*)alarmtempe,"%f",&tmp);
		//设置好报警温度值，以度的10倍数表示
		set_alarm_tempe((int32_t)(tmp*10));
	}
	#endif
	bool any_key_pressed;
	dp("scankey_task_had_init ok");
	while(1)
	{
		delay_while(); 
		if(__C)
		{
			taskENTER_CRITICAL();
			{				 
				keyboard_not_locked = true;
				//不允许其他线程操作屏幕
				dp("键盘程序独占有屏幕");
				dp("解锁");
			}
			taskEXIT_CRITICAL();
			//显示菜单
			dp("显示菜单");
			ui_menu();
			
			static bool break1 = false;//加static防止重复分配空间
			while(1)
			{
				delay_while();	
				any_key_pressed = true;         
				if(__K)
				{
					dp("K键");
					if(is_gotosetphone())//要设置电话号码
					{
						//设置之前用一个临时号码变量
						strncpy((char*)tmpalarmphone,(char*)alarmphone,12);
						strncpy((char*)tmpalarmtempe,(char*)alarmtempe,5);
						tmpalarmphone[11]=0;
						tmpalarmtempe[4]=0;
						//进入设置电话号码界面
						ui_setphone();
						dp("电话号码设置界面");
						////static bool break2 = false;
						while(1)
						{
							delay_while();
							any_key_pressed = true;         
							if(__T)
							{
								ui_tip_user(6,3,"<unsaved>"); 
								dp("T键");
								//号码位加一
								ui_setphoneup();
							}else if(__B)
							{
								ui_tip_user(6,3,"<unsaved>"); 
								dp("B键");
								//号码位减一
								ui_setphonedown();
							}else if(__L)
							{	
								dp("L键");
								//光标左移一位
								ui_setphoneprev();
							}else if(__R)
							{
								
								dp("R键");
								//光标右移一位
								ui_setphonenext();
							}else if(__F)
							{
								dp("F键");
								//返回上一级菜单
								ui_menu();
								break;
							}else if(__K)
							{
								dp("K键");
								dp("更新报警号码");
								//显示正在更新电话号码界面
								ui_updatephone();
								static bool is_update_ok;
								tmpalarmphone[11] = 0;
								tmpalarmtempe[4] = 0;
								//更新电话号码程序
								update_alarm_phone_tempe();
								is_update_ok = true;/*	总是认为更新成功	*/
								
								//进入设置电话号码界面
								ui_setphone();
								dp("电话号码设置界面");
								
								if(is_update_ok)
								{
									//保存回原变量
									strncpy((char*)alarmphone,(char*)tmpalarmphone,12);
									
									dp("设置报警号码为%s\r\n",alarmphone);
									ui_tip_user(6,3,"<saved>"); 
								}else
								{
									vTaskDelay(pdMS_TO_TICKS(50));
									ui_tip_user(6,3,"<try later!>"); 
								}
								
							}else if(__C)
							{
								
								dp("C键");
								dp("上锁");
								break1 = true;//继续跳出第一层循环
								break;
							}else
								any_key_pressed = false;
							if(any_key_pressed)
								feed_dog_for_secure_state();
						}
						if(break1 == true)
						{
							
							break1 = false;
							break;
						}
					}else if(is_gotosettempe())//要设置报警温度
					{
				
						//设置之前用一个临时号码变量
						strncpy((char*)tmpalarmphone,(char*)alarmphone,12);
						strncpy((char*)tmpalarmtempe,(char*)alarmtempe,5);
						tmpalarmphone[11]=0;
						tmpalarmtempe[4]=0; 
						//printf("447::tmpalarmtempe::%s\r\n", tmpalarmtempe);
						dp("报警温度设置界面");
						//进入设置报警温度界面
						ui_settempe();
						//printf("451::tmpalarmtempe::%s\r\n", tmpalarmtempe);
						////static bool break2 = false;
						while(1)
						{
							delay_while();
							any_key_pressed = true;         
							if(__T)
							{
								ui_tip_user(6,3,"<unsaved>"); 
								dp("T键");
								//号码位加一
								ui_settempeup();
							}else if(__B)
							{
								ui_tip_user(6,3,"<unsaved>"); 
								dp("B键");
								//号码位减一
								ui_settempedown();
							}else if(__L)
							{
								
								dp("L键");
								//光标左移一位
								ui_settempeprev();
							}else if(__R)
							{
			
								dp("R键");
								//光标右移一位
								ui_settempenext();
							}else if(__F)
							{
								dp("F键");
								//返回上一级菜单
								ui_menu();
								break1 = false;
								break;
							}else if(__C)
							{
								dp("C键");
								dp("上锁");
								break1 = true;//继续跳出
								break;
							}else if(__K)
							{
								dp("K键");
								dp("更新报警温度");
								//显示正在更新报警温度的界面
								ui_updatetempe();
								tmpalarmphone[11] = 0;
								tmpalarmtempe[4] = 0;
								static bool is_update_ok;
								//更新报警温度到flash中保存						
								update_alarm_phone_tempe();
								is_update_ok = true;/*	总是认为更新成功	*/									
								//进入设置报警温度界面
								dp("进入报警温度设置界面");
								ui_settempe();
							
								if(is_update_ok)
								{
									//保存新值
									strncpy((char*)alarmtempe,(char*)tmpalarmtempe,5);
									static double tmp;
									sscanf((char*)alarmtempe,"%lf",&tmp);
									//更新报警温度程序
									set_alarm_tempe(tmp*10);
									dp("设置报警温度为：%lf",tmp);
									ui_tip_user(6,3,"<saved>"); 
								}else
								{
									vTaskDelay(pdMS_TO_TICKS(50));
									ui_tip_user(6,3,"<try later!>"); 
								}
							}else        
							{
								any_key_pressed = false;         
							}
							if(any_key_pressed)
							{
								feed_dog_for_secure_state();
							}
						}
						if(break1 == true)
						{
							break1 = false;
							break;
						}
					}else if(is_gotowindowops() == true)
					{
						dp("进入设置窗户开关界面");
						ui_windowops();
						while(1)
						{
							delay_while();
							any_key_pressed = true;         
							if(__L)//L键，箭头左指
							{	
								ui_windowopsprev();
								if(get_windowstatus() != WIN_CLOSE)	
								{
									/*关窗操作*/
									close_window();
									while(!(__READ_L))
									{	
										feed_dog_for_secure_state();
										/*等待开窗完成*/
										if(get_windowstatus() == WIN_CLOSE)	
										{	
											taskENTER_CRITICAL();
											stop_window();
											taskEXIT_CRITICAL();							
											break;
										}
										vTaskDelay(pdMS_TO_TICKS(10));
										dp("L键未释放 val = %d val2 = %d",0,0);
									}
									dp("停止窗户.......................\r\n");
									taskENTER_CRITICAL();
									stop_window();
									taskEXIT_CRITICAL();		
								}									
							}
							else if(__R)//R键，箭头右指示 
							{
								ui_windowopsnext();
	
								if(get_windowstatus() != WIN_OPEN)
								{
									/*开窗操作*/
									open_window();
									while(!(__READ_R))
									{	
										feed_dog_for_secure_state();
										/*等待开窗完成*/
										if(get_windowstatus() == WIN_OPEN)	
										{
											taskENTER_CRITICAL();
											stop_window();
											taskEXIT_CRITICAL();
											break;
										}
										vTaskDelay(pdMS_TO_TICKS(10));
										dp("R键未释放 val = %d val2 = %d",0,0);
									}
									dp("停止窗户.......................\r\n");
									taskENTER_CRITICAL();
									stop_window();
									taskEXIT_CRITICAL();							
								}						
							}
							else if(__F)//F键，返回上一级菜单
							{
								dp("进入主菜单界面");
								ui_menu();
								break1 = false;//不继续跳出第一层循环
								break;
							}else if(__C)//C键，退出设置
							{
								break1 = true;//继续跳出第一层循环
								break;
							}else 
								any_key_pressed = false;         
							if(any_key_pressed)	
								feed_dog_for_secure_state();
						}           
						if(break1 == true)
						{
							dp("从窗户操作界面继续跳出");
							break;
						}
					}else if(is_gotodustwinops() == true)
					{
						//进入防雾霾窗纱操作界面
						ui_dustwinops();
						while(1)
						{
							delay_while();
							any_key_pressed = false;         							 
							if(__L)//L键
							{
								dp("箭头前指");
								ui_dustwinopsprev();
								if(get_dustwinstatus() != DUSTWIN_CLOSE)	
								{
									/*关窗操作*/
									close_preventdustwin(0);
									while(!(__READ_L))
									{	
										feed_dog_for_secure_state();
										/*等待开窗完成*/
										if(get_dustwinstatus() == DUSTWIN_CLOSE)	
										{	
											taskENTER_CRITICAL();
											stop_preventdustwin();
											taskEXIT_CRITICAL();							
											break;
										}
										vTaskDelay(pdMS_TO_TICKS(10));
//										dp("L键未释放 val = %d val2 = %d",val,val2);
									}
									dp("停止窗户.......................\r\n");
									taskENTER_CRITICAL();
									stop_preventdustwin();
									taskEXIT_CRITICAL();		
								}					
							}else if(__R)
							{
								dp("箭头后指");
								ui_dustwinopsnext();
								if(get_dustwinstatus() != DUSTWIN_OPEN)
								{
									/*开窗操作*/
									open_preventdustwin(0);
									while(!(__READ_R))
									{	
										feed_dog_for_secure_state();
										/*等待开窗完成*/
										if(get_dustwinstatus() == DUSTWIN_OPEN)	
										{
											taskENTER_CRITICAL();
											stop_preventdustwin();
											taskEXIT_CRITICAL();
											break;
										}
										vTaskDelay(pdMS_TO_TICKS(10));
										//dp("R键未释放 val = %d val2 = %d",val,val2);
									}
									dp("停止窗户.......................\r\n");
									taskENTER_CRITICAL();
									stop_preventdustwin();
									taskEXIT_CRITICAL();							
								}						
								
							}else if(__F)
							{
								//返回上一级菜单
								ui_menu();
								dp("返回上一级菜单");
								break1 = false;
								break;
							}else if(__C)
							{
								//键盘上锁
								break1 = true;
								break;
							}else 
								any_key_pressed = false;         
							if(any_key_pressed)	
								feed_dog_for_secure_state();
						}
						if(break1 == true)
						{
							dp("继续跳出");
							break;
						}
					}
				}else if(__T)
				{
					dp("T键");
					//上一个选项
					ui_menuprev();
				}else if(__B)
				{
					dp("B键");
					//下一个选项
					ui_menunext();  
				}else if(__C)
				{
					dp("C键");
					dp("键盘上锁");
					break;//返回到键盘上锁之前的状态
				}else 
					any_key_pressed = false;         
				if(any_key_pressed)	
					feed_dog_for_secure_state();
			}
		}else 
		{
			taskENTER_CRITICAL();
			{
				if(keyboard_not_locked == true)
				{
					dp("键盘程序释放屏幕");
					//允许其他进程操作屏幕
					keyboard_not_locked = false;
				}
			}
			taskEXIT_CRITICAL();
		}
	}
}

