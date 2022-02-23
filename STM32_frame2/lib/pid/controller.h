#ifndef _controller_h_
#define _controller_h_

#include "stm32f10x.h"

#define  throttle_median 1000
#define  BUFFER_TIME 150
typedef struct 
{
    int out;
}_OUT_Motor;

typedef struct 
{
    uint16_t val;
    uint8_t  to_left_flag;
    uint8_t  to_right_flag;
    
}_JUMP_DEFINE;

extern _OUT_Motor Motor1;
extern _OUT_Motor Motor2;

extern char deal_buff[13];

extern float track_err;
extern float set_speed;
extern float car_speed;
extern char cnt;

void motor_speed(void);

extern short X;
extern short W;

void track_deal(void);

void track_deal2(void);

void motor_control(float _err);


void angle_controller(void);
void gyro_controller(void);
void _controller_output(void);
void _controller_perform(void);
    
extern _OUT_Motor Motor1;
#endif

