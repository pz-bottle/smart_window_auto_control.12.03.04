#include "gsm.h"

extern u8 gsm_rxbuf[];

extern u8 gsm_index;

void gsm_init(void)
{
	GPIO_InitTypeDef  io;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	io.GPIO_Pin = GPIO_Pin_12;				      
	io.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	io.GPIO_Speed = GPIO_Speed_50MHz;		
	//gsm 复位引脚
	GPIO_Init(GPIOB, &io);	
	GPIOB->ODR &= ~(1<<12);
	
	USART3_Init(115200);
}

/************************************************************************************
 * 函数名称：gsm_send_at
 * 功能描述：gsm模块发送指令
 * 输入参数：- cmd：指令（包括回车换行）
 *	    - response：期望响应字符串
 *	    - wait_ms：等待响应的时间 单位/毫秒
 * 输出参数：无
 * 函数返回：发送指令成功true 否则false
 ************************************************************************************/
bool gsm_send_at(u8 cmd[],u8 response[],u32 wait_ms)
{
	//重置串口接收区
	memset(gsm_rxbuf,'\0',GSM_RXBUF_SIZE);
	gsm_index 	= 0;
	//发送命令
	gsm_send_str(cmd);
	//检查命令是否成功执行
	while(wait_ms != 0)
	{
		if(strstr((char*)gsm_rxbuf,(char*)response) != NULL)
		{
			return true;
		}
		gsm_delay_ms(2);
		wait_ms--;
	}
	return false;
}
/************************************************************************************
 * 函数名称：gsm_send_message
 * 功能描述：gsm模块发送信息
 * 输入参数：- msg：信息内容（英文）
 *	    - phone：目标电话号码
 * 输出参数：无
 * 函数返回：发送成功true 否则false
 ************************************************************************************/
s8 gsm_send_message(u8 *msg,u8 *phone)
{
	u8 end[2]={0x1A,0};//启动发送短信
	u8 tmp[50];

	//配置为TEXT模式,发送英文短信
	if(gsm_send_at("AT+CMGF=1\r\n","OK",49) == false)
		return -1;
	
	//设置字符格式
	if(gsm_send_at("AT+CSCS=\"GSM\"\r\n","OK",49) == false)
		return -2;
	
	//设置收信人号码
	sprintf((char*)tmp,"AT+CMGS=\"%s\"\r\n",phone);
	if(gsm_send_at(tmp,">",49) == false)
		return -3;
	
	//写短信内容
	gsm_send_str(msg);
	
	//启动发送
	if(gsm_send_at(end,"OK",49) == false)
		return -4;
	return 0;
}
/************************************************************************************
 * 函数名称：gsm_set_namecard
 * 功能描述：设置名片信息
 * 输入参数：- phone：电话号码
 *	    - name：姓名
 *	    - index：存储索引
 * 输出参数：无
 * 函数返回：设置成功true 否则false
 ************************************************************************************/
s8 gsm_set_namecard(u8 phone[],u8 name[],u32 index)
{
	if(gsm_send_at("AT+CPBS=\"SM\"\r\n","OK",49) != true)
	{
		//printf("buf is %s\r\n",usart3_rxbuf);
		//printf("fail at AT+CPBS.\n");
		return -1;
	}
	u8 buf[50];
	sprintf((char*)buf,"AT+CPBW=%d,\"%s\",129,\"%s\"\r\n",index,phone,name);
	
	//printf("dddddddddddddddddddddd---->%s\r\n",buf);
	if(gsm_send_at(buf,"OK",49) != true)
		return -2;
	return 0;
}
/************************************************************************************
 * 函数名称：gsm_get_namecard
 * 功能描述：获取名片信息
 * 输入参数：- index：存储索引 
 * 输出参数：- phone：电话号码
 *	    - name：姓名
 * 函数返回：获取成功true 否则false
 ************************************************************************************/
s8 gsm_get_namecard(u8 phone[],u8 name[],u32 index)
{
	u8 *s,buf[50],i;
	//选择存储位置
	if(gsm_send_at("AT+CPBS=\"SM\"\r\n","OK",49) != true)
	{
		//printf("fail at AT+CPBS\r\n");
		return -1;
	}
	//位置
	sprintf((char*)buf,"AT+CPBR=%d\r\n",index);
	if(gsm_send_at(buf,"\",",49) != true)
	{
		//printf("buf is %s\r\n",usart3_rxbuf);
		//printf("fail at AT+CPBR");
		return -2;
	}
	//获得电话号码 姓名
	s = (u8*)strstr((char*)gsm_rxbuf,",\"");
	if(s != NULL)
	{
		//printf("dddddd---------%s\r\n",s);
		s += 2;//电话号码距此的偏移量，打印处理看看就知道怎么回事
		for(i=0; i<11; i++)
			phone[i] = *s++;
		//printf("phone:%s----\r\n",phone);
		phone[12] = 0;
		//printf("phone:%s----\r\n",phone);
		//return true;
		gsm_delay_ms(10);//等传送完成，gsm模块发送名片信息到单片机需要时间，不等待可能得不到数据
		s = (u8*)strstr((char*)s,(char*)",\"");
		s += 2;
		while(*s != '\"')
			*name++ = *s++;
		//printf("phone:%s----\r\n",phone);
		return 0;
	}
	
	return 0;
}
s8 gsm_call(s8 phone[])
{
	char buf[50];
	sprintf(buf,"ATD%s\r\n",phone);
	bool ok = gsm_send_at((u8*)buf,"OK",49);
	if(ok)
		return 0;
	return -1;
}
/*///////////////////////////////////////////////////////////////
AT+CPBS=? 查询支持的存储位置
AT+CPBS="SM" 选择存储位置"SM"
AT+CPBW=? 查询支持写名片的位置
AT+CPBW=1,"13570574320",129,"LJW" 写名片到1号位置，129代表国内电话
AT+CPBR=? 查询支持的位置，这个位置存放了名片
AT+CPBR=1 查询1号位置的名片信息
AT+CPBF="LJW" 找关键字威"LJW"的名片信息
///////////////////////////////////////////////////////////////
AT+CSCS=? 查看支持的字符集
AT+CSCS="GSM" 设置为"GSM"格式 
AT+CSCS="UCS32"
AT+CMGF=1设置短信格式
AT+CMGR=1读取短信
///////////////////////////////
ATD10086 拨打10086
ATH 挂断电话
*/

