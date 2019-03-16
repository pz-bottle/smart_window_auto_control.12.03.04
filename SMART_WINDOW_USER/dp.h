#ifndef __DP_H__  
#define __DP_H__
#include "stdio.h"
#include "freertos.h"
#include "task.h"
//#define __USE_DP__ 								
#ifdef __USE_DP__
	#define dp(format,...) do{\
		printf("line:%d file:%s ",__LINE__,__FILE__);\
		printf(format,##__VA_ARGS__);printf("\r\n");\
	}\
	while(0)
#else
	#define dp(format,...) (void)0		
#endif
#endif// __DP_H__
