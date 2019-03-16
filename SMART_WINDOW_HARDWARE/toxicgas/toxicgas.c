#include "toxicgas.h"
/*********************************************************************************************************************
 * 函数名称：toxicgas_init
 * 功能描述：初始化有毒气体检测
 * 输入参数：无
 * 输出参数：无
 * 函数返回：无
 *********************************************************************************************************************/
void toxicgas_init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef io;	
	//adc时钟分频
	RCC_ADCCLKConfig(RCC_PCLK2_Div2); 
	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//AD 输入
	io.GPIO_Pin = GPIO_Pin_5;
	io.GPIO_Mode = GPIO_Mode_AIN;
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
u32 toxicgas_getad(u8 ch)
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
