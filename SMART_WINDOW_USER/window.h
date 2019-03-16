#ifndef __WINDOW_H__
#define __WINDOW_H__
#include "stm32f10x.h"
#include "delay.h"
#define DUSTWIN_CLOSE 0x1
#define DUSTWIN_OPEN  0x0
#define DUSTWIN_UNKNOWN 0xaa
#define WIN_CLOSE     0x1
#define WIN_OPEN      0x0
#define WIN_UNKNOWN   0xaa
void rainwiper_reset(void);
void set_servo_win_left(uint32_t val);
void set_servo_win_right(uint32_t val);
void close_window(void);
void open_window(void);
void stop_window(void);
s8 get_windowstatus(void);   
void close_preventdustwin(int32_t val);
void open_preventdustwin(int32_t val);
void stop_preventdustwin(void);
s8 get_dustwinstatus(void);
#endif//__WINDOW_H__
