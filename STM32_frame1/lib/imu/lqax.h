#ifndef _LQAX_H
#define _LQAX_H

#include "sys.h"
#include "softiic.h"

#define OUT_X_MSB_REG         0x01
#define OUT_X_LSB_REG         0x02
#define OUT_Y_MSB_REG         0x03
#define OUT_Y_LSB_REG         0x04
#define OUT_Z_MSB_REG         0x05
#define OUT_Z_LSB_REG         0x06

/////////////////2100////////////////////////////
#define FX_2100_ADDR    0x20  //地址
#define FX_READ         0x41
#define FX_WRITE        0x40

#define FX_STATUS       0X00

#define FA_X_MSB        0X01
#define FA_X_LSB        0X02
#define FA_Y_MSB        0X03
#define FA_Y_LSB        0X04
#define FA_Z_MSB        0X05
#define FA_Z_LSB        0X06

#define FA_DR_STATUS    0X07   //0X00
#define FA_F_STATUS     0X08   //0X00
#define FA_F_SETUP      0X09   //RW
#define FA_EVENT        0X0A
#define FA_INT_SRC_FLAG 0X0B
#define FA_WHO_AM_I     0X0C
#define FA_CTRL_REG0    0X0D  //RW
#define FA_RT_CFG       0X0E  //RW
#define FA_RT_SRC       0X0F
#define FA_RT_THS       0X10  //RW
#define FA_RT_COUNT     0X11  //RW
#define FA_TEMP         0X12

#define FX_CTRL_REG1    0X13  //RW
#define FX_CTRL_REG2    0X14  //RW
#define FX_CTRL_REG3    0X15  //RW

//////////////8700////////////////////////////////
#define FXOS_8700_ADDR          0x1e

#define FXOS8700CQ_STATUS       0x00

#define FXOS8700CQ_WHOAMI       0x00

#define FXOS8700CQ_XYZ_DATA_CFG 0x0E

#define FXOS8700CQ_CTRL_REG1    0x2A

#define FXOS8700CQ_M_CTRL_REG1  0x5B

#define FXOS8700CQ_M_CTRL_REG2  0x5C

#define FXOS8700CQ_WHOAMI_VAL   0xC7

/*******************************************************
					获取角速度
*******************************************************/
#define GetGyroX() 	Read_Dat(FX_2100_ADDR,FA_X_MSB)
#define GetGyroY()	Read_Dat(FX_2100_ADDR,FA_Y_MSB)
#define GetGyroZ() 	Read_Dat(FX_2100_ADDR,FA_Z_MSB)

/*******************************************************
				  获取角加速度
*******************************************************/
#define GetAccX()	Read_Dat(FXOS_8700_ADDR,FA_X_MSB)
#define GetAccY()	Read_Dat(FXOS_8700_ADDR,FA_Y_MSB)
#define GetAccZ()	Read_Dat(FXOS_8700_ADDR,FA_Z_MSB)

extern uint8 gyro_enable_flag;
extern float gyro_zero;
extern float ang_acc;
extern float ang_gyro;
extern float angle;
extern float ang1;
extern float any_z;

void Init_2100_8700(void);
int16 Read_Dat(uint8 slave,uint8 reg);
float Get_Ang_vel(uint8 n);
float Get_Po_Ang(void);
float Ang_measure(void);
float Get_Ang(void);








#endif
