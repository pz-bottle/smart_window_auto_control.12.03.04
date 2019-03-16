#include <pca9685.h>

/*_2caddr:保存i2c设备地址*/
static uint8_t _i2caddr;
/**
  * @brief   
  * @param   
  * @param   
  * @retval 
  */
uint8_t PCA9685_read8(uint8_t addr) 
{
	IIC_Start();
	IIC_Send_Byte((_i2caddr<<1)|0); 	
	IIC_Wait_Ack();

	IIC_Send_Byte(addr); 
	IIC_Wait_Ack();


	IIC_Stop();	
	IIC_Start();

	IIC_Send_Byte((_i2caddr<<1)|1); 
	IIC_Wait_Ack(); 

	uint8_t ret = IIC_Read_Byte(0);
	IIC_Stop();     
	return ret;
}
/**
  * @brief   
  * @param   
  * @param   
  * @retval 
  */
void PCA9685_write8(uint8_t addr, uint8_t d) 
{
	IIC_Start();     
	IIC_Send_Byte((_i2caddr<<1)|0); 	
	IIC_Wait_Ack();
	
	IIC_Send_Byte(addr); 
	IIC_Wait_Ack();

	IIC_Send_Byte(d); 
	IIC_Wait_Ack();

	IIC_Stop();     
}
/**
  * @brief   
  * @param   
  * @param   
  * @retval 
  */
void PCA9685_reset(void) 
{
	PCA9685_write8(PCA9685_MODE1, 0x0);
}
/**
  * @brief  设置引脚输出PWM
  * @param   
  * @param   
  * @retval 
  */
void PCA9685_setPWMFreq(float freq) 
{
	float prescaleval = 25000000;
	prescaleval /= 4096;
	prescaleval /= freq;
	prescaleval -= 1;
	uint8_t prescale = floor(prescaleval + 0.5);
	uint8_t oldmode = PCA9685_read8(PCA9685_MODE1);
	uint8_t newmode = (oldmode&0x7F) | 0x10; // sleep
	PCA9685_write8(PCA9685_MODE1, newmode); // go to sleep
	PCA9685_write8(PCA9685_PRESCALE, prescale); // set the prescaler
	PCA9685_write8(PCA9685_MODE1, oldmode);
	delay_us(5);
	PCA9685_write8(PCA9685_MODE1, oldmode | 0xa1);
}
/**
  * @brief  设置引脚输出PWM
  * @param   
  * @param   
  * @retval 
  */
void PCA9685_setPWM(uint8_t num, uint16_t on, uint16_t off) 
{
  
	IIC_Start();     
	IIC_Send_Byte((_i2caddr<<1)|0); 	
	IIC_Wait_Ack();
	
	IIC_Send_Byte(LED0_ON_L+4*num); 
	IIC_Wait_Ack();

	IIC_Send_Byte(on); 
	IIC_Wait_Ack();

	IIC_Send_Byte(on>>8); 
	IIC_Wait_Ack();

	IIC_Send_Byte(off); 
	IIC_Wait_Ack();

	IIC_Send_Byte(off>>8); 
	IIC_Wait_Ack();
	
	IIC_Stop();
}
/**
  * @brief  设置引脚输出PWM
  * @param  num: 硬件号，值为0-16
  * @param  val: 周期高电平的宽度
  * @param  pwmfreq: 输出PWM的频率 
  * @retval none
  */
void PCA9685_setPin(uint8_t num, uint16_t val)
{
	// Clamp value between 0 and 4095 inclusive.
	val = (val<4095)?val:4095;
	if (val == 4095) 
	{
	  // Special value for signal fully on.
	  PCA9685_setPWM(num, 4096, 0);
	}
	else if (val == 0) 
	{
	  // Special value for signal fully off.
	  PCA9685_setPWM(num, 0, 4096);
	}
	else 
	{
	  PCA9685_setPWM(num, 0, val);
	}
	
}
/**
  * @brief  初始化
  * @param  dev_addr: i2c设备的地址
  * @param  pwmfreq: 输出PWM的频率 
  * @retval none
  */
uint32_t _cnt_per_ms;
void PCA9685_init(uint8_t dev_addr, float  pwmfreq) 
{
	_i2caddr = dev_addr;//0x40;
	IIC_Init();
	PCA9685_reset();
	PCA9685_setPWMFreq(pwmfreq);
	_cnt_per_ms = 4096/(1000.0/pwmfreq);//计算1个毫秒的计数值
	
	for(int i=0; i<16; i++)
		PCA9685_setPin(i, 0);
}
void PCA9685_set_pulsh_ms(uint8_t num, float val)//设置脉冲宽度,单位ms
{
	PCA9685_setPin(num, val*_cnt_per_ms);
}

