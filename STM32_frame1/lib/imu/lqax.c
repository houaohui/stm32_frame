#include "lqax.h"
#include "myinclude.h"
#include "math.h"
float angle = 0;				//鏈�缁堣搴�
float ang_acc = 0;				
float ang_gyro = 0;
float gyro_time = 0.005;		//绉垎鏃堕棿
float lvbo_k = 0.005;			//浜掕ˉ婊ゆ尝绯绘暟
float ang_vel_x_zero = 1.5;		//闄�铻轰华闆跺亸 X杞�
float ang_vel_y_zero = 1.3;		//闄�铻轰华闆跺亸 y杞�
float ang_vel_z_zero = -5.08;      //闄�铻轰华闆跺亸 z杞�
float any_z=0;  //妫�娴嬪潯閬撶敤

uint8 gyro_enable_flag=0;
/*
 * @brief:  鍒濆鍖栧姞閫熷害璁″拰闄�铻轰华
 * @param:  void
 * @retval: void
 */
void Init_2100_8700(void)
{
	IMU2_IIC_Init();
	IMU2_WriteRegister(FXOS_8700_ADDR,FXOS8700CQ_CTRL_REG1, 0x00);
  	IMU2_WriteRegister(FXOS_8700_ADDR,FXOS8700CQ_CTRL_REG1, 0x0f);
  	IMU2_WriteRegister(FXOS_8700_ADDR,FXOS8700CQ_M_CTRL_REG2,0x20);
  	IMU2_WriteRegister(FXOS_8700_ADDR,FXOS8700CQ_XYZ_DATA_CFG,0x01);
  	IMU2_WriteRegister(FXOS_8700_ADDR,FXOS8700CQ_CTRL_REG1,0x0d);
  	//2100鍒濆鍖�
  	IMU2_WriteRegister(FX_2100_ADDR,FX_CTRL_REG3, 0x40);
  	IMU2_WriteRegister(FX_2100_ADDR,FX_CTRL_REG1, 0x01);
  	delay_ms(1);
  	IMU2_WriteRegister(FX_2100_ADDR,FX_CTRL_REG1,0x03);
}

/*
 * @brief:  鏁版嵁璇诲彇鍑芥暟
 * @param:  浠庢満鍦板潃銆佸瘎瀛樺櫒鍦板潃
 * @retval: 16浣嶆暟鎹�
 */
int16 Read_Dat(uint8 slave,uint8 reg)
{
	int16 MSB,LSB;
        gpio_dir (GPIOB, 11,GPO);
  	MSB = IMU2_ReadRegister(slave,reg);
	LSB = IMU2_ReadRegister(slave,reg+1);
	
	MSB = MSB << 8;
	MSB |= LSB;
	
	return MSB;
}

/*
 * @brief:      璁＄畻濮挎�佽
 * @param:      void
 * @retval:     void
 * @describe:   浜掕ˉ婊ゆ尝锛堟竻鍗庢柟妗堬級
 */
float Ang_measure(void)				//閫氳繃瑙掑姞閫熷害璁＄畻鍑鸿搴�
{
	float ang_vel_y;
  	ang_vel_y = Get_Ang();
	ang_gyro += ang_vel_y * gyro_time;	//瑙掗�熷害绉垎寰楀埌瑙掑害
//  	ang_acc = Get_Po_Ang();
//	if(ang_acc >= ang_gyro+10)			//鍙樺寲闄愬箙
//    	ang_acc = ang_gyro+10;
//  	else if(ang_acc <= ang_gyro-10)
//    	ang_acc = ang_gyro-10;
//	ang_err = ang_acc - ang_gyro;
//  	ang_gyro += ang_err * lvbo_k;		//浜掕ˉ婊ゆ尝
//	angle = ang_gyro - 3;
  	return ang_gyro;
}

/*
 * @brief:      鑾峰彇瑙掗�熷害
 * @param:      void
 * @retval:     瑙掗�熷害
 * @describe:   宸茬粡鎹㈢畻杩囧崟浣嶏紙闄�16.4锛�
 */
float Get_Ang_vel(uint8 n)
{
  float ang_vel=0;
    if(n == 1)
    {
        ang_vel = GetGyroZ() - ang_vel_z_zero;
       // printf("%f\n",ang_vel) ;
    }
    else if(n == 2)
    {
	
        ang_vel = GetGyroX() - ang_vel_x_zero;
       // printf("%f\n",ang_vel) ;
	
    }
    return ang_vel;
}

/*
 * @brief:      鑾峰彇瑙掗�熷害
 * @param:      void
 * @retval:     瑙掗�熷害
 * @describe:   宸茬粡鎹㈢畻杩囧崟浣嶏紙闄�16.4锛�
 */
float Get_Ang(void)
{
	float ang_vel=0;
        ang_vel = (GetGyroZ()- ang_vel_z_zero)/16.4;
       // printf("%f\n",ang_vel) ;
	return ang_vel;
}


float Get_Po_Ang(void)
{
    float x,y,ang_acc;
    y = GetAccY() - ang_vel_y_zero;
    x = GetAccX() - ang_vel_x_zero;
    ang_acc = -(57.3*atan2(x,y));     // 180/蟺=57.3

    if (ang_acc >90)
    {
      ang_acc = 90.0;
    }
    if(ang_acc<-90)
      ang_acc = -90.0;
   
    return ang_acc; 
}





