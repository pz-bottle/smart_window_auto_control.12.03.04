#ifndef __FLASH_H
#define __FLASH_H
#include <stm32f10x.h>
void flash_write(uint32_t start_addr, uint8_t out[], uint32_t size);
void flash_read(uint32_t start_addr, uint8_t out[], uint32_t size);
#define FLASH_PAGE_SIZE 2048
#define FLASH_PAGE_255_START_ADDR 0x0807F800
#define FLASH_PAGE_255_END_ADDR   0x0807FFFF

/*保存电话号码用12个字节，字符串形式保存："13570574320" */
#define FLAHS_SAVE_ALARM_PHOME_ADDR	(FLASH_PAGE_255_END_ADDR 	 - 12)
/*保存温度用4个字节，字符串形式保存："23.6"*/
#define FLASH_SAVE_ALARM_TEMP_ADDR	(FLAHS_SAVE_ALARM_PHOME_ADDR - 5)
/*保存湿度用4个字节，字符串形式保存："88.6"*/
#define FLASH_SAVE_ALARM_HUMI_ADDR	(FLASH_SAVE_ALARM_TEMP_ADDR  - 5)           
#endif//__FLASH_H
