#ifndef _SOFTIIC_H
#define _SOFTIIC_H
#include "sys.h"
#include "common.h"
#define IMU2_SCL  P10_1
#define IMU2_SDA  P10_2

typedef unsigned char   uint8;

#define SDAIN2        PBin(11)


#define I2C2_SDA_H    PBout(11)=1;
#define I2C2_SCL_H    PBout(10)=1;


#define I2C2_SDA_L    PBout(11)=0;
#define I2C2_SCL_L    PBout(10)=0;


#define SDA2_Out  gpio_dir(GPIOB,11,GPO);
#define SDA2_In   gpio_dir(GPIOB,11,GPI);

void IMU2_IIC_Init(void);
void IIC2_start(void);
void IIC2_stop(void);
void Write_IIC2_Byte(unsigned char IIC_Byte);
uint8 IIC2_read_byte(void);
void  IMU2_WriteRegister(uint8 SlaveID, uint8 reg, uint8 Data);
uint8 IMU2_ReadRegister(uint8 SlaveID, uint8 reg);

#endif /*_SOFTIIC_H*/
