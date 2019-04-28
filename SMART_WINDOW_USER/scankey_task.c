#include "scankey_task.h"
static volatile bool keyboard_not_locked = false;//��Ļ�Ƿ��Ѿ�����
static s8 ui_menuindex = 0;
static s8 ui_setphoneindex = 1;/*һ��ʼ��ָ��绰����ĵ�1λ�����磺13570574320��ִ��3*/
static s8 ui_settempeindex = 0;
//static s8 ui_windowopsindex = 0;
static  u8 alarmtempe[5] = {""},tmpalarmtempe[5]={"12.3"};//������������浽stm32 flash
static  u8 alarmphone[12] = {""},tmpalarmphone[12]={"13570574320"};//������������浽stm32 flash
bool is_operatable_screen(void)//���˼��̳�����������ֻ�ܻ�ȡ��Ļ�Ƿ�ɲ���״̬�����д����д����������ṩ����ӿ�
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
//���˵�
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
//���ǰ��
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
//������
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
//���ñ�������
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
//�������
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
//�������
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
//��һ
void ui_setphoneup(void)
{
	s8 i = ui_setphoneindex;
	tmpalarmphone[i] = (tmpalarmphone[i]-'0'+1+10)%10 + '0';
	taskENTER_CRITICAL();
	LcdWriteString(6,1,tmpalarmphone);
	taskEXIT_CRITICAL();
}
//��һ
void ui_setphonedown(void)
{
	s8 i = ui_setphoneindex;
	tmpalarmphone[i] = (tmpalarmphone[i]-'0'-1+10)%10 + '0';
	taskENTER_CRITICAL();
	LcdWriteString(6,1,tmpalarmphone); 
	taskEXIT_CRITICAL();
}
//����
void ui_updatephone(void)
{
	taskENTER_CRITICAL();
	LcdWriteString(6,1,"updating...");
	taskEXIT_CRITICAL();
}
//���ñ����¶�
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
//�������
void ui_settempeprev(void)
{
	s8 i = ui_settempeindex;
	taskENTER_CRITICAL();
	{
		LcdWriteString((i+1)*6,2," ");
		i = (i-1+4)%4;
		if(i == 2)//����С����
			i = (i-1+4)%4;
		LcdWriteString((i+1)*6,2,"^");
		printf("ui_settempeprev::%d\r\n", i);
	}
	taskEXIT_CRITICAL();	
	ui_settempeindex = i;
	
}
//�������
void ui_settempenext(void)
{
	s8 i = ui_settempeindex;
	taskENTER_CRITICAL();
	{
		LcdWriteString((i+1)*6,2,(u8*)" ");
		i = (i+1+4)%4;
		if(i == 2)//����С����
			i = (i+1+4)%4;
		LcdWriteString((i+1)*6,2,(u8*)"^");
		//printf("ui_settempenext::%d\r\n", i);
	}
	taskEXIT_CRITICAL();
	ui_settempeindex = i;
	
}
//��һ
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
//��һ
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
//����
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
//���ǰָ
void ui_windowopsprev(void)
{
	taskENTER_CRITICAL();
	LcdWriteString(0,3,"  <<<=======");
	taskEXIT_CRITICAL();
}
//����ָ
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

//���ǰָ
void ui_dustwinopsprev(void)
{
	taskENTER_CRITICAL();
	LcdWriteString(0,3,"  <<<=======");
	taskEXIT_CRITICAL();
}
//����ָ
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
enum
{
	KEYBOARD_LOCKED_UI=1, /*��������ʱ��UI*/
	FIRST_UI = KEYBOARD_LOCKED_UI,
	MENU_UI,		/*�˵�UI*/
	SET_PHONE_UI,		/*���ñ����绰UI*/
	SET_TEMP_UI,		/*���ñ����¶�UI*/
	WINDOW_OPS_UI,		/*�ⴰ������UI*/
	DUST_WIN_OPS_UI,	/*��������������UI*/
};							
static uint8_t get_ui(void);	
static void set_ui(int ui);
static uint8_t xxxx_keyboardlocked_ui(void);
static uint8_t xxxx_menu_ui(void);
static uint8_t xxxx_dustwinops_ui(void);
static uint8_t xxxx_windowsops_ui(void);
static uint8_t xxxx_setphone_ui(void);
static uint8_t xxxx_settemp_ui(void);
bool is_digit_string_verify(char digit_string[]);
void scankey_task_init(void);
void scankey_task(void *parameter)
{
	scankey_task_init();
	////////////////////////////////////////////////////////////////////////////////////////////////
	while(1)
	{
		/*�����������UI*/
		if(get_ui() == KEYBOARD_LOCKED_UI) 
		{
			set_ui(xxxx_keyboardlocked_ui());
		}
		
		if(get_ui() == MENU_UI)
		{
			set_ui(xxxx_menu_ui());
		}
		
		if(get_ui() == DUST_WIN_OPS_UI)
		{
			set_ui(xxxx_dustwinops_ui());
		}
		
		if(get_ui() == WINDOW_OPS_UI)
		{
			set_ui(xxxx_windowsops_ui());
		}	
		
		if(get_ui() == SET_PHONE_UI)
		{
			set_ui(xxxx_setphone_ui());
		}
		
		if(get_ui() == SET_TEMP_UI)     
		{
			set_ui(xxxx_settemp_ui());
		}
		
	}
}
static uint8_t __current_ui;
static uint8_t get_ui(void)
{
	return __current_ui;
}	
static void set_ui(int ui)
{
	__current_ui = ui;
}
void scankey_task_init(void)
{
	LcdReset();
	LcdClear();
	dp("scankey_task start");
	float tmp;
	dp("׼��ι��");
	/*��ɨ��������񴴽��ɹ���ż���ִ��*/
	// while((eTaskGetState( watchdog_for_secure_state_task ) != eReady))
	while((eTaskGetState( watchdog_for_secure_state_task_handle ) != eReady))
	{
		dp("watchdog_for_secure_state_task not ok.\r\n");
		vTaskDelay(pdMS_TO_TICKS(10));
	}
	dp("ready");
	feed_dog_for_secure_state();
	dp("ι���ɹ�");
	#if 1
	if(scankey_task_had_init == false)/*���û�г�ʼ��������ʼ��һ��*/
	{
		scankey_task_had_init = true;/*��һ���Ѿ���ʼ������*/	
		/*	��ȡ������flash�е�ֵ,�����ĵ绰����	*/
		flash_read(FLAHS_SAVE_ALARM_PHOME_ADDR, (u8*)alarmphone,12);
		if( ! is_digit_string_verify((char*)alarmphone) )
		{
			snprintf((char*)alarmphone, 12, "13570574320");
		}
		/*	��ȡ������flash�е�ֵ,�������¶�	*/
		flash_read(FLASH_SAVE_ALARM_TEMP_ADDR, (u8*)alarmtempe, 5);
		if( ! is_digit_string_verify((char*)tmpalarmtempe) )
		{
			snprintf((char*)alarmtempe, 5, "25.0");
		}
		/*	ɨ�赽�����б���	*/
		sscanf((char*)alarmtempe,"%f",&tmp);
		//���úñ����¶�ֵ���Զȵ�10������ʾ
		set_alarm_tempe((int32_t)(tmp*10));
	}
	#endif
	set_ui(FIRST_UI);
	dp("scankey_task_had_init ok");
}
static inline bool is_digit(char c)
{
	return (c<='9' && c>='0');
}
bool is_digit_string_verify(char digit_string[])
{
	bool result = true;
	for(int i=0; digit_string[i]!='\0'; i++)
	{
		if( ! is_digit(digit_string[i]) )
		{
			result = false;
			break;
		}
	}
	return result;
}
static uint8_t xxxx_keyboardlocked_ui(void)
{
	bool any_key_pressed;
	uint8_t next_ui = KEYBOARD_LOCKED_UI;
	while(1)
	{
		vTaskDelay(pdMS_TO_TICKS(10)); 
		if(__C)
		{
			taskENTER_CRITICAL();
			{				 
				keyboard_not_locked = true;
				//�����������̲߳�����Ļ
				dp("���̳����ռ����Ļ");
				dp("����");
			}
			taskEXIT_CRITICAL();
			//��ʾ�˵�
			dp("��ʾ�˵�");
		    next_ui = MENU_UI;
			break;
		}else 
		{
			taskENTER_CRITICAL();
			{
				if(keyboard_not_locked == true)
				{
					dp("���̳����ͷ���Ļ");
					//�����������̲�����Ļ
					keyboard_not_locked = false;
				}
			}
			taskEXIT_CRITICAL();
		}
	}
	return next_ui;
}
static uint8_t xxxx_menu_ui(void)
{
	bool any_key_pressed;
	uint8_t next_ui = MENU_UI;
	ui_menu();
	static bool break1 = false;//��static��ֹ�ظ�����ռ�
	while(1)
	{
		vTaskDelay(pdMS_TO_TICKS(10));	
		any_key_pressed = true;         
		if(__K)
		{
			dp("K��");
			if(is_gotosetphone())//Ҫ���õ绰����
			{
				next_ui = SET_PHONE_UI;
				break;
			}else if(is_gotosettempe())//Ҫ���ñ����¶�
			{
				next_ui = SET_TEMP_UI;
				break;
			}else if(is_gotowindowops() == true)
			{
				next_ui = WINDOW_OPS_UI;
				break;
			}else if(is_gotodustwinops() == true)
			{
				next_ui = DUST_WIN_OPS_UI;
				break;
			}
		}else if(__T)
		{
			dp("T��");
			//��һ��ѡ��
			ui_menuprev();
		}else if(__B)
		{
			dp("B��");
			//��һ��ѡ��
			ui_menunext();  
		}else if(__C)
		{
			dp("C��");
			dp("��������");
			next_ui = KEYBOARD_LOCKED_UI;
			break;//���ص���������֮ǰ��״̬
		}else 
			any_key_pressed = false;         
		if(any_key_pressed)	
			feed_dog_for_secure_state();
	}
	return next_ui;
}
static uint8_t xxxx_dustwinops_ui(void)
{
	bool any_key_pressed;
	uint8_t next_ui = DUST_WIN_OPS_UI;
	//�����������ɴ��������
	ui_dustwinops();
	while(1)
	{
		vTaskDelay(pdMS_TO_TICKS(10));
		any_key_pressed = false;         							 
		if(__L)//L��
		{
			dp("��ͷǰָ");
			ui_dustwinopsprev();
			if(get_dustwinstatus() != DUSTWIN_CLOSE)	
			{
				/*�ش�����*/
				close_preventdustwin(0);
				while(!(__READ_L))
				{	
					feed_dog_for_secure_state();
					/*�ȴ��������*/
					if(get_dustwinstatus() == DUSTWIN_CLOSE)	
					{	
						taskENTER_CRITICAL();
						stop_preventdustwin();
						taskEXIT_CRITICAL();							
						break;
					}
					vTaskDelay(pdMS_TO_TICKS(10));
//										dp("L��δ�ͷ� val = %d val2 = %d",val,val2);
				}
				dp("ֹͣ����.......................\r\n");
				taskENTER_CRITICAL();
				stop_preventdustwin();
				taskEXIT_CRITICAL();		
			}					
		}else if(__R)
		{
			dp("��ͷ��ָ");
			ui_dustwinopsnext();
			if(get_dustwinstatus() != DUSTWIN_OPEN)
			{
				/*��������*/
				open_preventdustwin(0);
				while(!(__READ_R))
				{	
					feed_dog_for_secure_state();
					/*�ȴ��������*/
					if(get_dustwinstatus() == DUSTWIN_OPEN)	
					{
						taskENTER_CRITICAL();
						stop_preventdustwin();
						taskEXIT_CRITICAL();
						break;
					}
					vTaskDelay(pdMS_TO_TICKS(10));
					//dp("R��δ�ͷ� val = %d val2 = %d",val,val2);
				}
				dp("ֹͣ����.......................\r\n");
				taskENTER_CRITICAL();
				stop_preventdustwin();
				taskEXIT_CRITICAL();							
			}						
			
		}else if(__F)
		{
			//������һ���˵�
			dp("������һ���˵�");
			next_ui = MENU_UI;
			break;
		}else if(__C)
		{
			//��������
			next_ui = KEYBOARD_LOCKED_UI;
			break;
		}else 
			any_key_pressed = false;         
		if(any_key_pressed)	
			feed_dog_for_secure_state();
	}
	return next_ui;
}
static uint8_t xxxx_windowsops_ui(void)
{
	bool any_key_pressed;
	uint8_t next_ui = WINDOW_OPS_UI;
	dp("�������ô������ؽ���");
	ui_windowops();
	while(1)
	{
		vTaskDelay(pdMS_TO_TICKS(10));
		any_key_pressed = true;         
		if(__L)//L������ͷ��ָ
		{	
			ui_windowopsprev();
			if(get_windowstatus() != WIN_CLOSE)	
			{
				/*�ش�����*/
				close_window();
				while(!(__READ_L))
				{	
					feed_dog_for_secure_state();
					/*�ȴ��������*/
					if(get_windowstatus() == WIN_CLOSE)	
					{	
						taskENTER_CRITICAL();
						stop_window();
						taskEXIT_CRITICAL();							
						break;
					}
					vTaskDelay(pdMS_TO_TICKS(10));
					dp("L��δ�ͷ� val = %d val2 = %d",0,0);
				}
				dp("ֹͣ����.......................\r\n");
				taskENTER_CRITICAL();
				stop_window();
				taskEXIT_CRITICAL();		
			}									
		}
		else if(__R)//R������ͷ��ָʾ 
		{
			ui_windowopsnext();
			if(get_windowstatus() != WIN_OPEN)
			{
				/*��������*/
				open_window();
				while(!(__READ_R))
				{	
					feed_dog_for_secure_state();
					/*�ȴ��������*/
					if(get_windowstatus() == WIN_OPEN)	
					{
						taskENTER_CRITICAL();
						stop_window();
						taskEXIT_CRITICAL();
						break;
					}
					vTaskDelay(pdMS_TO_TICKS(10));
					dp("R��δ�ͷ� val = %d val2 = %d",0,0);
				}
				dp("ֹͣ����.......................\r\n");
				taskENTER_CRITICAL();
				stop_window();
				taskEXIT_CRITICAL();							
			}						
		}
		else if(__F)//F����������һ���˵�
		{
			dp("�������˵�����");
			next_ui = MENU_UI;
			break;
		}else if(__C)//C�����˳�����
		{
			next_ui = KEYBOARD_LOCKED_UI;
			break;
		}else 
			any_key_pressed = false;         
		if(any_key_pressed)	
			feed_dog_for_secure_state();
	}           
	return next_ui;
}
static uint8_t xxxx_setphone_ui(void)
{
	bool any_key_pressed;
	uint8_t next_ui;
	//����֮ǰ��һ����ʱ�������
	strncpy((char*)tmpalarmphone,(char*)alarmphone,12);
	strncpy((char*)tmpalarmtempe,(char*)alarmtempe,5);
	tmpalarmphone[11]=0;
	tmpalarmtempe[4]=0;
	//�������õ绰�������
	ui_setphone();
	dp("�绰�������ý���");
	////static bool break2 = false;
	while(1)
	{
		vTaskDelay(pdMS_TO_TICKS(10));
		any_key_pressed = true;         
		if(__T)
		{
			ui_tip_user(6,3,"<unsaved>"); 
			dp("T��");
			//����λ��һ
			ui_setphoneup();
		}else if(__B)
		{
			ui_tip_user(6,3,"<unsaved>"); 
			dp("B��");
			//����λ��һ
			ui_setphonedown();
		}else if(__L)
		{	
			dp("L��");
			//�������һλ
			ui_setphoneprev();
		}else if(__R)
		{
			
			dp("R��");
			//�������һλ
			ui_setphonenext();
		}else if(__F)
		{
			dp("F��");
			//������һ���˵�
			next_ui = MENU_UI;
			break;
		}else if(__K)
		{
			dp("K��");
			dp("���±�������");
			//��ʾ���ڸ��µ绰�������
			ui_updatephone();
			static bool is_update_ok;
			tmpalarmphone[11] = 0;
			tmpalarmtempe[4] = 0;
			//���µ绰�������
			update_alarm_phone_tempe();
			is_update_ok = true;/*	������Ϊ���³ɹ�	*/
			
			//�������õ绰�������
			ui_setphone();
			dp("�绰�������ý���");
			
			if(is_update_ok)
			{
				//�����ԭ����
				strncpy((char*)alarmphone,(char*)tmpalarmphone,12);
				
				dp("���ñ�������Ϊ%s\r\n",alarmphone);
				ui_tip_user(6,3,"<saved>"); 
			}else
			{
				vTaskDelay(pdMS_TO_TICKS(50));
				ui_tip_user(6,3,"<try later!>"); 
			}
			
		}else if(__C)
		{	
			dp("C��");
			dp("����");
			next_ui = KEYBOARD_LOCKED_UI;
			break;
		}else
			any_key_pressed = false;
		if(any_key_pressed)
			feed_dog_for_secure_state();
	}
	return next_ui;
}
static uint8_t xxxx_settemp_ui(void)
{
	bool any_key_pressed;
	uint8_t next_ui = SET_TEMP_UI;
	//����֮ǰ��һ����ʱ�������
	strncpy((char*)tmpalarmphone,(char*)alarmphone,12);
	strncpy((char*)tmpalarmtempe,(char*)alarmtempe,5);
	tmpalarmphone[11]=0;
	tmpalarmtempe[4]=0; 
	//printf("447::tmpalarmtempe::%s\r\n", tmpalarmtempe);
	dp("�����¶����ý���");
	//�������ñ����¶Ƚ���
	ui_settempe();
	//printf("451::tmpalarmtempe::%s\r\n", tmpalarmtempe);
	////static bool break2 = false;
	while(1)
	{
		vTaskDelay(pdMS_TO_TICKS(10));
		any_key_pressed = true;         
		if(__T)
		{
			ui_tip_user(6,3,"<unsaved>"); 
			dp("T��");
			//����λ��һ
			ui_settempeup();
		}else if(__B)
		{
			ui_tip_user(6,3,"<unsaved>"); 
			dp("B��");
			//����λ��һ
			ui_settempedown();
		}else if(__L)
		{
			
			dp("L��");
			//�������һλ
			ui_settempeprev();
		}else if(__R)
		{

			dp("R��");
			//�������һλ
			ui_settempenext();
		}else if(__F)
		{
			dp("F��");
			//������һ���˵�
			next_ui = MENU_UI;
			break;
		}else if(__C)
		{
			dp("C��");
			dp("����");
			next_ui = KEYBOARD_LOCKED_UI;
			break;
		}else if(__K)
		{
			dp("K��");
			dp("���±����¶�");
			//��ʾ���ڸ��±����¶ȵĽ���
			ui_updatetempe();
			tmpalarmphone[11] = 0;
			tmpalarmtempe[4] = 0;
			static bool is_update_ok;
			//���±����¶ȵ�flash�б���						
			update_alarm_phone_tempe();
			is_update_ok = true;/*	������Ϊ���³ɹ�	*/									
			//�������ñ����¶Ƚ���
			dp("���뱨���¶����ý���");
			ui_settempe();
		
			if(is_update_ok)
			{
				//������ֵ
				strncpy((char*)alarmtempe,(char*)tmpalarmtempe,5);
				static double tmp;
				sscanf((char*)alarmtempe,"%lf",&tmp);
				//���±����¶ȳ���
				set_alarm_tempe(tmp*10);
				dp("���ñ����¶�Ϊ��%lf",tmp);
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
	return next_ui;
}
