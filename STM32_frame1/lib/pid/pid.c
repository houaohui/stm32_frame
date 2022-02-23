#include "pid.h"

_PID motorL;
_PID motorR;
_PID turn;

//存储pid控制器参数
const float  controller_parameter[3][5] =
{
 //0.kp 1.ki 2.kd 3.积分限幅  4.pid输出限幅值                                               
    {6, 0.0,  0,  500 , 800 },                //motorL              
    {6, 0.0,  0,  500 , 800 },                //motorR  
	{5, 0,  2,  500 , 500 },	
};

//pid参数初始化配置
void pid_init(_PID *controller,uint8_t label)
{
    controller->kp              = controller_parameter[label][0];         
    controller->ki              = controller_parameter[label][1];         
    controller->kd              = controller_parameter[label][2];         
    controller->integral_max    = controller_parameter[label][3];         
    controller->out_max         = controller_parameter[label][4];               
}
//pid参数初始化
void all_pid_init(void)
{
    pid_init(&motorL,0);
    pid_init(&motorR,1);
	pid_init(&turn,2);
} 
//pid控制器
float pid_controller(_PID *controller)
{
    controller->err_last = controller->err;                                                 //保存上次偏差
    controller->err = controller->expect - controller->feedback;                            //偏差计算
    controller->integral += controller->ki * controller->err;                               //积分  
    //积分限幅
    if(controller->integral >  controller->integral_max)     controller->integral =  controller->integral_max;
    if(controller->integral < -controller->integral_max)     controller->integral = -controller->integral_max;
    //pid运算
    controller->out =  controller->kp*controller->err + controller->integral + controller->kd*(controller->err-controller->err_last);
    //输出限幅
    if(controller->out >  controller->out_max)   controller->out =  controller->out_max;
    if(controller->out < -controller->out_max)   controller->out = -controller->out_max;
    return controller->out;
}
//清除积分
void clear_integral(_PID *controller)
{
    controller->integral = 0.0f;
}
