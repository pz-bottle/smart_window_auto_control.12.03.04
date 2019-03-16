#include "nrf24l01.h"  
#include "stdio.h" 
const u8 TX_ADDRESS [TX_ADR_WIDTH]={0x60,0x20,0x30,0x40,0x50}; //发送地址 

const u8 RX_ADDRESS0[RX_ADR_WIDTH]={0x10,0x20,0x30,0x40,0x50}; //接收地址  
const u8 RX_ADDRESS1[RX_ADR_WIDTH]={0x20,0x20,0x30,0x40,0x50}; //接收地址  
const u8 RX_ADDRESS2[1           ]={0x30}; //接收地址  
const u8 RX_ADDRESS3[1           ]={0x40}; //接收地址  
const u8 RX_ADDRESS4[1           ]={0x50}; //接收地址  
const u8 RX_ADDRESS5[1           ]={0x60}; //接收地址  

/*************************************************
* 函数名称：InitNrf24L01
* 功能描述：初始NRF24L01，包括对IRQ引脚的初始化
* 输入参数：无
* 输出参数：无
* 返回值：  返回0成功，返回1失败
**************************************************/ 
u8 InitNrf24L01(void)  
{   
    GPIO_InitTypeDef GPIO_InitStructure;  
	//使能GPIO 的时钟              
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOG,ENABLE);    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;        //推挽输出  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(GPIOB, &GPIO_InitStructure);   
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8;  
	GPIO_Init(GPIOG, &GPIO_InitStructure);   
    //
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  
    GPIO_Init(GPIOG, &GPIO_InitStructure);   
    //芯片失能
    NRF_CE_H;
    //没有中断
    IRQ_H;
    //SPI失能
    CSN_H;
    //MUC对nrf的中断配置
    NrfIsrCfg();
    return Nrf24L01Check();
}  

/*************************************************
* 函数名称：SpiReadWriteByte
* 功能描述：主、从机交换8位数据
* 输入参数：- TxData：主机发送的数据
* 输出参数：无
* 返回值：  从机发送的数据
**************************************************/  
u8 SpiReadWriteByte(u8 TxData)                                          
{         
    u8 i; 
    NRF_SCK_L;//拉低时钟
    for(i=0;i<8;i++)   
    {  
        if(TxData & 0x80)  
        MOSI_H;          
        else  
        MOSI_L;
        //移空最低位，丢弃最高位   
        TxData = (TxData << 1);    
        //写从设备       
        NRF_SCK_H;  
        //读从设备       
        if(READ_MISO)   
        //保存读取的数据                
        TxData |= 0x01;         
        //拉低时钟线.完成一次数据交换（SR和BUF）        
         NRF_SCK_L;                                
    }  
    return(TxData);                               
}  

/*************************************************
* 函数名称：Nrf24l01ReadWriteBus
* 功能描述：主、从机交换多个字节数据
* 输入参数：- cmd：指令；
		    - data：主机发送的数据
* 输出参数：- data：从机发送的数据
* 返回值：  无
**************************************************/ 
void Nrf24l01ReadWriteBus(u8 cmd,u8 data[],u8 cnt)
{
	//1、使能SPI传输
	//2、发送指令
	//3、发送、接收数据
	//4、失能SPI传输
	u8 i;
	CSN_L;
	SpiReadWriteByte(cmd);
	for(i = 0; i < cnt; i++)
	{
		data[i] = SpiReadWriteByte(data[i]);
	}
	CSN_H;
}
/*************************************************
* 函数名称：Nrf24L01Check
* 功能描述：检测NRF24L01是否存在
* 输入参数：无
* 输出参数：无
* 返回值：  返回0存在，返回1不存在
**************************************************/ 
u8 Nrf24L01Check(void)  
{  
    u8 a[5]={0XA0,0XA1,0XA2,0XA3,0XA4};  
    u8 b[5];  
    u8 i;          
    for(i=0;i<5;i++)b[i]=a[i];
	//写入5个字节.    
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+TX_ADDR,a,5);
    //读出写入的5个字节   
    Nrf24l01ReadWriteBus(SPI_READ_REG+TX_ADDR,a,5);  
    for(i=0;i<5;i++)
    {
        if(a[i]!=b[i]) break;    
    }    
    if(i!=5) return 1;                               //NRF24L01不在位        
    return 0;                                       //NRF24L01在位  
}                             
/*************************************************
* 函数名称：Nrf24L01SendPacket
* 功能描述：启动NRF24L01发送一次数据  
* 输入参数：- tx_buf：要发送的数据
* 输出参数：无
* 返回值：  无
**************************************************/ 
void Nrf24L01SendPacket(u8 *tx_buf)  
{  
    u8 result,data[TX_PLOAD_WIDTH];  
    //失能芯片，进入配置模式
    NRF_CE_L;  
    //数据复制，不希望原数据被更改
    for(result=0;result<TX_PLOAD_WIDTH;result++)
		data[result]=tx_buf[result];
	//写数据到发送缓冲区TX BUF  32个字节  
    Nrf24l01ReadWriteBus(WR_TX_PLOAD,data,TX_PLOAD_WIDTH);
	//使能芯片，启动发送  
    NRF_CE_H;                                     
}  
                           
/*************************************************
* 函数名称：Nrf24l01GetPacketHandler
* 功能描述：NRF24L01接收数据中断处理函数    
* 输入参数：- rx_buf：接收数据的数组
* 输出参数：- rx_buf: 接收到的数据
* 返回值：  接收的通道号
**************************************************/ 
u8 Nrf24l01GetPacketHandler(u8 *rx_buf)  
{  
    u8 result,channel;               
	//读取状态寄存器的值                                    
    Nrf24l01ReadWriteBus(SPI_READ_REG+STATUS,&result,1);
    //得到接收的通道号
    channel=((result&(7<<1))>>1);
    //读取数据  
    Nrf24l01ReadWriteBus(RD_RX_PLOAD,rx_buf,RX_PLOAD_WIDTH);
    //清除中断标志位
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+STATUS,&result,1);  
    //清空接收FIFO,读取之后自动清空，若不读取，必须清空，\
    否则nrf24l01无法正常工作：不能产生中断信号
    //Nrf24l01ReadWriteBus(FLUSH_RX,&result,1);   
    //返回接收的通道号
    return channel;
}  
                          
/*************************************************
* 函数名称：Nrf24l01SendSuccessHandler
* 功能描述：发送数据成功中断处理函数    
* 输入参数：无
* 输出参数：无
* 返回值：  无
**************************************************/ 
void Nrf24l01SendSuccessHandler(void)
{
    u8 data;
    //读取状态寄存器
    Nrf24l01ReadWriteBus(SPI_READ_REG+STATUS,&data,1);
    //清除所有中断标志位
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+STATUS,&data,1);    
    //清除TX FIFO寄存器,发送成功，自动清除   
    // Nrf24l01ReadWriteBus(FLUSH_TX,&data,1); 
}
                          
/*************************************************
* 函数名称：Nrf24l01SendFailHandler
* 功能描述：发送数据失败中断处理函数    
* 输入参数：无
* 输出参数：无
* 返回值：  无
**************************************************/ 
void Nrf24l01SendFailHandler(void)
{
    u8 data;
    //读取状态寄存器
    Nrf24l01ReadWriteBus(SPI_READ_REG+STATUS,&data,1);
    //清除所有中断标志位
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+STATUS,&data,1);    
    //清除TX FIFO寄存器,发送失败，必须手动清除，否则nrf24l01不能正常工作    
    Nrf24l01ReadWriteBus(FLUSH_TX,&data,1); 
}
                        
/*************************************************
* 函数名称：Nrf24l01GetMode
* 功能描述：配置NRF24L01到接收模式         
* 输入参数：无
* 输出参数：无
* 返回值：  无
**************************************************/               
void Nrf24l01GetMode(void)  
{ 
	u8 data[RX_ADR_WIDTH],i;
    //芯片失能，模块进入配置模式
    NRF_CE_L;
    //写RX节点地址，注意，是先写低位地址的，通道0可以拥有独立的40位地址。通道1-5的最低位字节地址不一样，其他必须一样
	for(i=0;i<RX_ADR_WIDTH;i++) 
		data[i]=RX_ADDRESS0[i];
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+RX_ADDR_P0,data,RX_ADR_WIDTH);
	for(i=0;i<RX_ADR_WIDTH;i++) 
		data[i]=RX_ADDRESS1[i];
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+RX_ADDR_P1,data,RX_ADR_WIDTH); 
	
	data[0]=RX_ADDRESS2[0];
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+RX_ADDR_P2,data,1); 
	data[0]=RX_ADDRESS3[0];
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+RX_ADDR_P3,data,1); 
	data[0]=RX_ADDRESS4[0];
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+RX_ADDR_P4,data,1); 
	data[0]=RX_ADDRESS5[0];
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+RX_ADDR_P5,data,1); 
	
    //使能通道0-5的自动应答,根据手册默认都是是开启的，实际测试发现并没有，可能是其他因素所致  
    data[0] = 0x3f;
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+EN_AA,data,1);
	
    //使能通道0-5的接收地址，通道0,1默认的开启的       
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+EN_RXADDR,data,1);  
	data[0] = 0x3f;
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+EN_RXADDR,data,1);
	
	//选择通道0的有效数据宽度         
	data[0] = RX_PLOAD_WIDTH;
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+RX_PW_P0,data,1); 
	//选择通道1的有效数据宽度      
	data[0] = RX_PLOAD_WIDTH;
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+RX_PW_P1,data,1);  
	//选择通道2的有效数据宽度        
	data[0] = RX_PLOAD_WIDTH;
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+RX_PW_P2,data,1);  
	//选择通道3的有效数据宽度        
	data[0] = RX_PLOAD_WIDTH;
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+RX_PW_P3,data,1);  
	//选择通道4的有效数据宽度       
	data[0] = RX_PLOAD_WIDTH;
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+RX_PW_P4,data,1);  
	//选择通道5的有效数据宽度       
	data[0] = RX_PLOAD_WIDTH;
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+RX_PW_P5,data,1);  
	
        
    //射频通道0-6位          
	data[0] = 0x4f;
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+RF_CH,data,1);        
    //0db增益,2Mbps,低噪声增益开启    
	data[0]=0;
    //bit3：产生速率，1->2Mbps 0->1Mbps
    data[0]|=1<<3;
    //bit2-bit1:发射功率，\
    00：-18dBm 01：-12dBm 10：-6dBm 11：0dBm
    data[0]|=0<<1;
    //bit0:低噪声放大器增益，1开启0关闭
    data[0]|=0<<0;
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+RF_SETUP,data,1);  
    data[0]=0;
    //bit6:接收中断，1屏蔽，0不屏蔽
	data[0]|=0<<6;
    //bit5:发送中断，1屏蔽，0不屏蔽
    data[0]|=0<<5;
    //bit4:达到最大重发中断，1屏蔽，0不屏蔽
	data[0]|=0<<4;
    //bit3:CRC使能，1使能，0不使能
    data[0]|=1<<3;
    //bit2:CRC模式，1->16位CRC校验，0->8位CRC校验
    data[0]|=1<<2;
    //bit1:1上电0掉电
    data[0]|=1<<1;
    //bit0:1接收模式0发送模式
    data[0]|=1<<0;
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+CONFIG, data,1);   
    //必须先清空接收FIFO否则nrf不能产生中断信号\
    如果此时mcu使用接收中断的方式处理，就无法进\
    入中断，IRQ一直保持低电平或高电平，若清除了\
    标志位，就一直保持高电平，否则一直保持低电\
    平。若使用查询方式，那么不必执行清空接收\
    FIFO但有可能第一次接收到错误的数据
    Nrf24l01ReadWriteBus(FLUSH_RX,data,1);
    //读取状态寄存器的值
    Nrf24l01ReadWriteBus(SPI_READ_REG+STATUS,data,1);
    //清除所有的标志位，否则进入工作模式后IRQ一直为低
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+STATUS,data,1);
    //芯片使能,进入接收模式   
    NRF_CE_H;                                  
}             
                       
/*************************************************
* 函数名称：Nrf24l01SendMode
* 功能描述：配置NRF24L01到发送模式         
* 输入参数：无
* 输出参数：无
* 返回值：  无
**************************************************/    
void Nrf24l01SendMode(void)  
{//      
	u8 data[TX_ADR_WIDTH],i;
    //地址复制，不希望原数据被更改
	for(i=0;i<TX_ADR_WIDTH;i++)
		data[i]=TX_ADDRESS[i];
    //失能芯片，进入配置模式
    NRF_CE_L;         
    //写TX节点地址  
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+TX_ADDR,data,TX_ADR_WIDTH);      
    //设置RX节点地址,一定是使用通道0的，在发送端，规定使用通道0发送数据和接收应答信号，收发地址是一样的才能正确应答  
	for(i=0;i<TX_ADR_WIDTH;i++)
		data[i]=TX_ADDRESS[i];
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+RX_ADDR_P0,data,TX_ADR_WIDTH);      
    //使能通道0的自动应答
	data[0]=1;
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+EN_AA,data,1);      
    //使能通道0的接收地址   
	data[0]=1;
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+EN_RXADDR,data,1);      
    //设置自动重发间隔时间,最大自动重发次数 
	data[0]=0x0;
    //bit7-bit4:自动重发延时，250*x+86us
    data[0]|=(0<<4);
    //bit3-bit0:自动重发次数
    data[0]|=(15<<0);
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+SETUP_RETR,data,1);  
    //设置RF通道bit0-bit6 实际通信频率=RF_CH+2400 (MHz)
	data[0]=0x4f;
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+RF_CH,data,1);   
    //0db增益,2Mbps,低噪声增益开启        
	data[0]=0;
    //bit3：产生速率，1->2Mbps 0->1Mbps
    data[0]|=1<<3;
    //bit2-bit1:发射功率，\
    00：-18dBm 01：-12dBm 10：-6dBm 11：0dBm
    data[0]|=0<<1;
    //bit0:低噪声放大器增益，1开启0关闭
    data[0]|=0<<0;
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+RF_SETUP,data,1);
    data[0]=0;
    //bit6:接收中断，1屏蔽，0不屏蔽
	data[0]|=0<<6;
    //bit5:发送中断，1屏蔽，0不屏蔽
    data[0]|=0<<5;
    //bit4:达到最大重发中断，1屏蔽，0不屏蔽
	data[0]|=0<<4;
    //bit3:CRC使能，1使能，0不使能
    data[0]|=1<<3;
    //bit2:CRC模式，1->16位CRC校验，0->8位CRC校验
    data[0]|=1<<2;
    //bit1:1上电0掉电
    data[0]|=1<<1;
    //bit0:1接收模式0发送模式
    data[0]|=0<<0;
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+CONFIG,data,1);
    //清空发送FIFO
    Nrf24l01ReadWriteBus(FLUSH_TX,data,1);   
    //读取状态寄存器
    Nrf24l01ReadWriteBus(SPI_READ_REG+STATUS,data,1);
    //清除所有中断标志位
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+STATUS,data,1);    
    //芯片使能，进入发送模式  
    NRF_CE_H;                                    
}   
                       
/*************************************************
* 函数名称：NrfIsrCfg
* 功能描述：配置NRF24L01 IRQ引脚中断io         
* 输入参数：无
* 输出参数：无
* 返回值：  无
**************************************************/  
void NrfIsrCfg(void)
{
    //1、时钟使能。
    //2、muc外部中断引脚的配置
    //3、中断优先级的配置
    GPIO_InitTypeDef GPIO_InitStructure;  
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    //开启复用功能时钟和io时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
    GPIO_Init(GPIOG,&GPIO_InitStructure);
    //配置中断源
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource6);
    //外部中断线配置
    EXTI_InitStruct.EXTI_Line=EXTI_Line6;
    EXTI_InitStruct.EXTI_LineCmd=ENABLE;
    EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;//下降沿触发
    EXTI_Init( &EXTI_InitStruct);
    //优先级配置
    NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0x3;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
    NVIC_Init(&NVIC_InitStruct);  
}

