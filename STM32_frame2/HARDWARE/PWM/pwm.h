#ifndef __PWM_H
#define __PWM_H
#include "sys.h"
#include "delay.h"
typedef enum
{
	steer1=0,
	steer2,
	steer3
}
STEER_enum;




//50hz
#define steer_mid  750
#define steer_max  1250
#define steer_min  250

#define STEER_RATIO  0.09




void TIM1_PWM_Init(u16 arr,u16 psc);
void TIM2_PWM_Init(u16 arr,u16 psc);


void Set_steerangle(STEER_enum steer,float angle);

#endif
