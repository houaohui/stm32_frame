#ifndef _pid_h_
#define _pid_h_

#include "sys.h"

typedef struct
{
    float err;
    float err_last;
    float expect;
    float feedback;
    float kp;
    float ki;
    float kd;
    float integral;
    float integral_max;
    float out;
    float out_max;
}_PID;

typedef struct
{
    //姿态外环
    _PID rol_angle;
    //编码器速度环
    _PID vel_encoder;
    _PID rol_gyro;
}_ALL_PID;

extern _PID motorL;
extern _PID motorR;
extern _PID turn;
float pid_controller(_PID *controller);
void all_pid_init(void);
void clear_integral(_PID *controller);

#endif
