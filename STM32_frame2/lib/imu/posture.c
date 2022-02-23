/*ͣ��*/
/**
 * @file posture.c
 * @brief �������Ǻͼ��ٶȼƽ�����̬
 * @mainpage ��̬����
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

float gyro_angle_x; //������X����ֽ��
float dt = 0.01;   //���ֵ�dt������2ms��������0.002
float acc_gyro_lvbo_index = 0.005;



/**
 * @brief ��ȡ������(gyro)��ԭʼ����
 * @details ע�⣺��ȡ��Ӧ�������ϵĽ��ٶ�w�����ǽǶȣ���
 *
 * @param  char axisn,�ᣬ��ȡxyz��XYZ
 * @param  float offsetn����Ư
 * @return 
 *     
 * @note Null
 */
 float get_gyro_axisn(char axisn,float offsetn)
 {
	 float w = 0;//���ٶ���w��ʾ
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
 * @brief ��ȡ���ٶȼ�(acceleromter)��ԭʼ����
 * @details ��ȡ��Ӧ����ÿ�����ϵļ��ٶȣ����磺���Լ���һ��оƬ
 ��z���ϣ��һ���û�����һ�����ٶ�ֵΪ1*g����Ϊ�������ٶ�
 *
 * @param  char axisn,�ᣬ��ȡxyz��XYZ
 * @param  float offsetn����Ư
 * @return a ���ٶ�

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
 * @brief �Ӽ��ٶȼƽ�����������ǣ�pitch��
 * @details ���յĸ������ɼ��ٶȼƺ������ǵ������ں�
 *
 * @param  NONE
 * @return pitch
   
 * @note Null
 */
 float get_pitch_from_acc(void)
 {
	 float y,z;//y,z�Ǿ��������ǵ�������
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
* @brief ��ȡ�ںϵĸ����ǣ�pitch�������������жϺ��������Ե��ã���
* @details ����ָ���ǳ�ͷ����ڳ�β��߶Ȳ�һ�¶������ĽǶ�
* 
* @param  
* @param 
* @return ����ĸ����Ƕ�

* @note ��ֵ�Ļ�ȡ����ʹ��[���ٶȼ���������]���������ƻ����ں�
* ����˼·��
1.������Ҫ���֣����Ҫ���ж���߿�һ·�����߳�,������ȡ���ֵõ��ĽǶ�
2.���ٶȼ����ڲ�����֣�ÿ�λ�ȡ����Ƕȵ�ʱ��ֱ�ӽ��㼴��
3.���ڴ����ִ������еõ��������Ƕȣ������Ƕȵ�ʱЧ�Բ�һ����
������ǿ����������������ݽ����ںϣ��õ�һ����Ҫ�Ľ�Ϊ׼ȷ������
4.�ںϷ�����ѡ�������ǵĽǶ���Ϊ���ζȽϸߵ�����a���ü��ٶȼƶ���������b�������ǵ�����aȡ�
֮�����ϵ��c�õ�(a-b)*c������һ���������ݣ���������������,a+(a-b)*c�����ں�֮��ĽǶȣ�
����c��Ҫ�ɾ���ʹ��������ֳ������ۺ�ȷ����
*/
float get_fushion_pitch(void)
{
	float gyro_angle_x_temp,acc_angle_temp,err;
	
	//�����ǽǶȴ���
	gyro_angle_x_temp = get_gyro_axisn('X',GYRO_X_OFFSET);
	gyro_angle_x += gyro_angle_x_temp*dt;
	
	//���ٶȼ����ݻ�ȡ
	acc_angle_temp = get_pitch_from_acc();
	
	//��������Ϊ�������Ƽ��ٶȼƵķ�Χ
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

