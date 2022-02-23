/*停用*/
/**
 * @file posture.c
 * @brief 由陀螺仪和加速度计解算姿态
 * @mainpage 姿态解算
 * @author hnlgEDDLinYu
 * @email 2970658553@qq.com
 * @version 1.0.0
 * @date 2020/7/23
 * @copyright    Copyright (c) hnlgEDDLinYu
 */
 
#include "posture.h"
#include "math.h"


float ACC_X_OFFSET =      1.5;
float ACC_Y_OFFSET =      1.3;
float ACC_Z_OFFSET =      10;

float GYRO_X_OFFSET =     1.5;
float GYRO_Y_OFFSET =     1.3;
float GYRO_Z_OFFSET =     10;

float gyro_angle_x; //陀螺仪X轴积分结果
float dt = 0.01;   //积分的dt，周期2ms，所以是0.002
float acc_gyro_lvbo_index = 0.005;



/**
 * @brief 获取陀螺仪(gyro)的原始数据
 * @details 注意：获取的应该是轴上的角速度w，不是角度！！
 *
 * @param  char axisn,轴，可取xyz或XYZ
 * @param  float offsetn，零漂
 * @return 
 *     
 * @note Null
 */
 float get_gyro_axisn(char axisn,float offsetn)
 {
	 float w = 0;//角速度用w表示
	if ((axisn == 'X')||( axisn == 'x'))
	{
		w = (GetGyroX() - offsetn)/16.4;
	}
	else if ((axisn == 'Y')||( axisn == 'y'))
	{
		w = (GetGyroY() - offsetn)/16.4;
	}
	else if ((axisn == 'Z')||( axisn == 'z'))
	{
		w = (GetGyroZ() - offsetn);
	}
	else
	{
		gui_print_s(0,LINE(10),"error get_gyro_axisn");
	}
	return (w);
 }
 
 
 /**
 * @brief 获取加速度计(acceleromter)的原始数据
 * @details 获取的应该是每个轴上的加速度，例如：我自己是一个芯片
 在z轴上，我会给用户反馈一个加速度值为1*g，即为重力加速度
 *
 * @param  char axisn,轴，可取xyz或XYZ
 * @param  float offsetn，零漂
 * @return a 加速度

 * @note Null
 */
 float get_acc_axisn(char axisn,float offsetn)
 {
	 float a = 0;
	if ((axisn == 'X')||( axisn == 'x'))
	{
		a = GetAccX()  - offsetn;
	}
	else if ((axisn == 'Y')||( axisn == 'y'))
	{
		a = GetGyroY() - offsetn;
	}
	else if ((axisn == 'Z')||( axisn == 'z'))
	{
		a = GetGyroZ() - offsetn;
	}
	else
	{
		gui_print_s(0,LINE(10),"error get_acc_axisn");
	}
	return (a);
 }
 /**
 * @brief 从加速度计解算出来俯仰角（pitch）
 * @details 最终的俯仰角由加速度计和陀螺仪的数据融合
 *
 * @param  NONE
 * @return pitch
   
 * @note Null
 */
 float get_pitch_from_acc(void)
 {
	 float y,z;//y,z是决定俯仰角的两个轴
	 float pitch;
	 y = get_acc_axisn('Y',ACC_X_OFFSET);
	 z = get_acc_axisn('Z',ACC_Z_OFFSET);
	 
	 pitch = -(57.3*atan2(y,z));
	 
	 if (pitch > 90)
	 {
		 pitch = 90;
	 }
	 else if (pitch < -90)
	 {
		 pitch = -90;
	 }
	 
	 return pitch;
 }
/**
* @brief 获取融合的俯仰角（pitch）函数，需在中断函数周期性调用！！
* @details 俯仰指的是车头相对于车尾因高度不一致而产生的角度
* 
* @param  
* @param 
* @return 算出的俯仰角度

* @note 该值的获取还是使用[加速度计与陀螺仪]的数据优势互补融合
* 总体思路：
1.陀螺仪要积分，因而要在中断里边开一路积分线程,用来获取积分得到的角度
2.加速度计由于不需积分，每次获取这个角度的时候，直接解算即可
3.现在从两种传感器中得到了两个角度，两个角度的时效性不一样，
因而我们可以利用这两个数据进行融合，得到一个想要的较为准确的数据
4.融合方法：选用陀螺仪的角度作为信任度较高的数据a，用加速度计读出的数据b与陀螺仪的数据a取差，
之后乘以系数c得到(a-b)*c，这是一个修正数据，用来修正陀螺仪,a+(a-b)*c就是融合之后的角度，
参数c需要由经验和传感器的现场表现综合确定。
*/
float get_fushion_pitch(void)
{
	float gyro_angle_x_temp,acc_angle_temp,err;
	
	//陀螺仪角度处理
	gyro_angle_x_temp = get_gyro_axisn('X',GYRO_X_OFFSET);
	gyro_angle_x += gyro_angle_x_temp*dt;
	
	//加速度计数据获取
	acc_angle_temp = get_pitch_from_acc();
	
	//以陀螺仪为主，限制加速度计的范围
	if (acc_angle_temp>=gyro_angle_x+10)
	{
		acc_angle_temp = gyro_angle_x+10;
	}
	else if (acc_angle_temp<=gyro_angle_x-10)
	{
		acc_angle_temp = gyro_angle_x-10;
	}
	
	err  = acc_angle_temp - gyro_angle_x;
	gyro_angle_x += err*acc_gyro_lvbo_index ;
	
	return (gyro_angle_x-4);
}

