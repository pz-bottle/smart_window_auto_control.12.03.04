
#include "dht22.h" 
static void DHT22Start(void); 

static u8 DHT22_Response(void); 	
 
static u8 DHT22ReadOneBit(void);			 

static u8 DHT22ReadOneByte(void);   

/***********************************************
 * 函数名称：DHT22Start
 * 功能描述：复位DHT22
 * 输入参数：无
 * 输出参数：无
 * 返回值：  无
************************************************/
static void DHT22Start(void)	   
{                 
	DHT22_DQ_OUT();//SET OUTPUT
	DHT22_DQ_L;//拉低DQ
	DHT22_DELAY_MS(1);   //拉低时间
	DHT22_DQ_H;//释放DQ
	DHT22_DQ_IN(); //SET INPUT
}
/***********************************************
 * 函数名称：DHT22_Response
 * 功能描述：检测DHT22是否响应
 * 输入参数：无
 * 输出参数：无
 * 返回值：  - 返回0：正常响应
 *		    - 返回1：异常响应
 ************************************************/
static u8 DHT22_Response(void) 	   
{   
	u8 retry=0;
	DHT22_DQ_IN();//SET INPUT	 
	while (READ_DQ&&retry<100)//DHT22会拉低40~80us
	{
		retry++;
		DHT22_DELAY_US(1);
	}	 
	if(retry>=100)return 1;
	else retry=0;
	while (!READ_DQ&&retry<100)//DHT22拉低后会再次拉高40~80us
	{
		retry++;
		DHT22_DELAY_US(1);
	}
	if(retry >= 100)return 1;	    
	return 0;
}

/***********************************************
* 函数名称：DHT22ReadOneBit
* 功能描述：从DHT22读取一个位
* 输入参数：无
* 输出参数：无
* 返回值：  读取的值，0或1
************************************************/
static u8 DHT22ReadOneBit(void) 			 
{
 	u8 retry=0;
	while(READ_DQ&&(retry<100))//等待变为低电平
	{
		retry++;
		DHT22_DELAY_US(1);
	}
	retry=0;
	while(!READ_DQ&&(retry<100))//等待变高电平
	{
		retry++;
		DHT22_DELAY_US(1);
	}
	DHT22_DELAY_US(40);//等待40us
	if(READ_DQ)return 1;
	else return 0;		   
}
/***********************************************
* 函数名称：DHT22ReadOneByte
* 功能描述：从DHT22读取一个字节
* 输入参数：无
* 输出参数：无
* 返回值：  读取的字节
************************************************/
static u8 DHT22ReadOneByte(void)    
{        
	u8 i,dat=0;
	for (i=0;i<8;i++) 
	{
		dat<<=1; //注意：先移位
		dat|=DHT22ReadOneBit();
	}						    
	return dat;
}
/***********************************************
* 函数名称：DHT22ReadTempeHumid
* 功能描述：从DHT22读取一个字节
* 输入参数：- temp：保存温度值地址
*			- humi：保存湿度值地址
* 输出参数：- tmep：保存温度值地址
*			- humi：保存湿度值地址
* 返回值：  返回0读取成功，1读取失败
************************************************/
u8 DHT22ReadTempeHumid(int *temp,int *humi)    
{        
 	u8 b[6];
	int i;
	DHT22Start();//发送开始信号
	if(DHT22_Response()==0)//等待响应
	{
		for(i=0;i<5;i++)//读取40位数据
		{
			b[i]=DHT22ReadOneByte();
		}
		
		//校验数据
		b[5] = (b[0]+b[1]+b[2]+b[3]);
		if(b[5]!=b[4])return 1;// 校验错误
		
		*humi = (int)((b[0]<<8)|b[1]);
		*temp = (int)((b[2]<<8)|b[3]);
		return 0;
	}
	
	return 1;
}
/***********************************************
* 函数名称：DHT22Init
* 功能描述：初始DHT22
* 输入参数：无
* 输出参数：无
* 返回值：  无
************************************************/
void DHT22Init(void)
{
	GPIO_InitTypeDef DQ_IO_InitStruct;
	DHT22_PORT_CLOCK(ENABLE);
	DQ_IO_InitStruct.GPIO_Pin =  (1<<DHT22_DQ_BIT);
	DQ_IO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	DQ_IO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( DHT22_PORT, &DQ_IO_InitStruct );	
	DHT22_DQ_H;//DQ线高电平，空闲状态
}

