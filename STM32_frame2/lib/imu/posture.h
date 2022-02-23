/*Í£ÓÃ*/
#ifndef _POSTURE_H
#define _POSTURE_H
#include "lqax.h"
#include "gui.h"


extern float ACC_X_OFFSET;
extern float ACC_Y_OFFSET;
extern float ACC_Z_OFFSET;

extern float GYRO_X_OFFSET;
extern float GYRO_Y_OFFSET;
extern float GYRO_Z_OFFSET;

extern float gyro_angle_x;

//pitch:¸©Ñö½Ç
//roll:ºá¹ö½Ç
//yaw:º½Ïò½Ç
extern float pitch;  //x
extern float roll;   //y
extern float yaw;    //z



 float get_gyro_axisn(char axisn,float offsetn);
 float get_acc_axisn(char axisn,float offsetn);
 float get_pitch_from_acc(void);
 float get_fushion_pitch(void);

#endif /*_POSTURE_H*/
