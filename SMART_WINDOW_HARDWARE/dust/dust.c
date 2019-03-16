#include "dust.h"
/*********************************************************************************************************************
 * 函数名称：dust_init
 * 功能描述：初始化灰尘模块
 * 输入参数：无
 * 输出参数：无
 * 函数返回：无
 *********************************************************************************************************************/
void dust_init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef io;	
	//adc时钟分频
	RCC_ADCCLKConfig(RCC_PCLK2_Div2); 
	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//AD 输入
	io.GPIO_Pin = GPIO_Pin_4;
	io.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &io);
	
	//LED 控制
	io.GPIO_Pin = GPIO_Pin_8;
	io.GPIO_Speed = GPIO_Speed_50MHz;
	io.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &io);
	
	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC1独有。ADC2\ADC3保留功能
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//单通道
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//非连续转换，需要一次触发一次即可
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//非外部触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据位右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);
	/* Enable ADC1 reset calibration register */   
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* Start ADC1 calibration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));
	/* Start ADC1 Software Conversion */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);  
}
u32 dust_getad(u8 ch)
{
	int i = 2;
	u16 t;
	while(i--)//不知道为什么要连续读读两次才行
	{
		ADC_RegularChannelConfig(ADC1,ch,1, ADC_SampleTime_239Cycles5);
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);  
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束 
		t = ADC_GetConversionValue(ADC1);//返回最近一次 ADC1 规则组的转换结果  ;
	}
	return t;
}
u32 dust_get_advalue(void )
{
	u32 val_ad;
	LED_H;//熄灭9.68毫秒
	delay_us(9680);
	LED_L;//点亮0.28毫秒
	delay_us(280);
	//ad采样
	val_ad = dust_getad(DUST_ADC_CH);
	delay_us(40);
	LED_H;//采集完毕，熄灭LED
	return val_ad;
}
//返回mg
double dust_get(uint32_t val)
{
	#if 0
	if(val>36)
		return ((val/1024.0)-0.0356)*120000*0.035;
	#endif
	#if 1
	double x,y;
	//x=(y-0.6)/((3.5-0.6)/0.49);mg/v
	y = (val*1.0)*3.6/4096;//条件：5v供电、12位adc 
	x = (y-0.6)/((3.5-0.6)/0.49);//通用公式
	return (x*(x>0?1:0));
	#endif
}
/*
x=0:0.01:0.45;
y = ((3.5-0.6)/0.49)*x+0.6;
plot(x,y,'-r');
grid on;
hold on;
plot(x,0.5*(x/x),'-k');
*/

