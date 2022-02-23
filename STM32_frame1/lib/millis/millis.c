#include "millis.h"


uint32_t running_tim_cnt = 0;



millis_t milliseconds=0;
millis_t millis_get()
{
	millis_t ms;
		ms = milliseconds;
	return ms;
}


//测试系统运行时间
void time_check(_Time_test *running)
{
    running->last_time_us = running->now_time_us;
    running->now_time_us = running_tim_cnt * 5000 + TIM1->CNT;                  //计数累加数增加1，则需要5ms = 5000us ，定时器定时5ms需要5000次计数，所以一次CNT值为1us 
    running->delta_time_us = running->now_time_us - running->last_time_us;    //时间差代尔塔
    running->delta_time_ms = running->delta_time_us * 0.001f;
}



