#include "softiic.h"
#include "myinclude.h"


void softiic_delay(int t)
{
	int i = t;
	while(i--);
}

static void nop(void)
{
	softiic_delay(100);
} 
static void nops(void)      
{
	softiic_delay(150);
}


/**
 * @brief    初始化IIC      
 * @param 
 * @return
 */
void IMU2_IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//使能GPIOB时钟
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}

/**
 * @brief    开始IIC传输
 * @param 
 * @return
 */
void IIC2_start(void)
{
    I2C2_SCL_L;
    I2C2_SDA_H;
    nop();
    I2C2_SCL_H;
    nops();
    I2C2_SDA_L;
    nops();
    I2C2_SCL_L;
}

/**
 * @brief    停止IIC传输
 * @param 
 * @return
 */
void IIC2_stop(void)
{
    I2C2_SCL_L;
    nop();
    I2C2_SDA_L;
    nop();
    I2C2_SCL_H;
    nops();
    I2C2_SDA_H;
    nops();
    I2C2_SCL_L;
}

/**
 * @brief    IIC数据发送 
 * @param    要发送的数据 
 * @return
 */
void Write_IIC2_Byte(unsigned char c)
{
    uint8 i;
    
    for(i = 0;i < 8;i++)
    {
        I2C2_SCL_L;
        if((c << i) & 0x80)
        {
          I2C2_SDA_H; //判断发送位
        }
        else
        {
          I2C2_SDA_L;
        }
        nop();
        I2C2_SCL_H;
        nops();
        I2C2_SCL_L;
    }
    nops();
    I2C2_SDA_H; //发送完8bit，释放总线准备接收应答位
    nop();
    I2C2_SCL_H;
    nops();     //sda上数据即是从应答位
    I2C2_SCL_L; //不考虑从应答位|但要控制好时序
}

/**
 * @brief    IIC读取数据
 * @param    
 * @return   读到的数据
 */
uint8 IIC2_read_byte(void)
{
    uint8 i;
    uint8 c;

    c = 0;
    I2C2_SCL_L;
    nop();
    I2C2_SDA_H; //置数据线为输入方式
    SDA2_In;
    for(i = 0;i < 8;i++)
    {
        nop();
        I2C2_SCL_L; //置时钟线为低，准备接收数据位
        nops();
        I2C2_SCL_H; //置时钟线为高，使数据线上数据有效
        nop();
        c <<= 1;
        if(SDAIN2)
          c += 1; //读数据位，将接收的数据存c
    }
    I2C2_SCL_L;
    SDA2_Out;
    return c;
}

/**
 * @brief    IIC写操作
 * @param    SlaveID 从机地址
 * @param    reg     寄存器地址
 * @param    Data    要写入的数据 
 * @return   
 */
void  IMU2_WriteRegister(uint8 SlaveID, uint8 reg, uint8 Data)
{
    IIC2_start();
    Write_IIC2_Byte(SlaveID << 1); //写入设备ID及写信号
    Write_IIC2_Byte(reg);
    Write_IIC2_Byte(Data);
    IIC2_stop();
}

/**
 * @brief    IIC读操作
 * @param    SlaveID 从机地址
 * @param    reg     寄存器地址
 * @return   读到的数据
 */
uint8 IMU2_ReadRegister(uint8 SlaveID, uint8 reg)
{
    uint8 ret;
    
    IIC2_start();
    Write_IIC2_Byte(SlaveID << 1); //写入设备ID及写信号
    Write_IIC2_Byte(reg);
    IIC2_start();
    Write_IIC2_Byte((SlaveID << 1) + 1); //写入设备ID及读信号
    ret = IIC2_read_byte();
    IIC2_stop();

    return ret;
}
