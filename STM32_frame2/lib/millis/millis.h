#ifndef _MILLIS_H_
#define _MILLIS_H_

#include "sys.h"



typedef unsigned int millis8_t;
typedef unsigned int  millis_t;

typedef struct
{
    float       last_time_us;
    float       now_time_us;
    float       delta_time_us;
    float       delta_time_ms;
}_Time_test;    


#define millis() millis_get()

extern  millis_t milliseconds;
extern uint32_t running_tim_cnt;


millis_t millis_get(void);
void time_check(_Time_test *running);


#endif

