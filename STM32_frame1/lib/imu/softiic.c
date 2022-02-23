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
 * @brief    ��ʼ��IIC      
 * @param 
 * @return
 */
void IMU2_IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//ʹ��GPIOBʱ��
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}

/**
 * @brief    ��ʼIIC����
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
 * @brief    ֹͣIIC����
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
 * @brief    IIC���ݷ��� 
 * @param    Ҫ���͵����� 
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
          I2C2_SDA_H; //�жϷ���λ
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
    I2C2_SDA_H; //������8bit���ͷ�����׼������Ӧ��λ
    nop();
    I2C2_SCL_H;
    nops();     //sda�����ݼ��Ǵ�Ӧ��λ
    I2C2_SCL_L; //�����Ǵ�Ӧ��λ|��Ҫ���ƺ�ʱ��
}

/**
 * @brief    IIC��ȡ����
 * @param    
 * @return   ����������
 */
uint8 IIC2_read_byte(void)
{
    uint8 i;
    uint8 c;

    c = 0;
    I2C2_SCL_L;
    nop();
    I2C2_SDA_H; //��������Ϊ���뷽ʽ
    SDA2_In;
    for(i = 0;i < 8;i++)
    {
        nop();
        I2C2_SCL_L; //��ʱ����Ϊ�ͣ�׼����������λ
        nops();
        I2C2_SCL_H; //��ʱ����Ϊ�ߣ�ʹ��������������Ч
        nop();
        c <<= 1;
        if(SDAIN2)
          c += 1; //������λ�������յ����ݴ�c
    }
    I2C2_SCL_L;
    SDA2_Out;
    return c;
}

/**
 * @brief    IICд����
 * @param    SlaveID �ӻ���ַ
 * @param    reg     �Ĵ�����ַ
 * @param    Data    Ҫд������� 
 * @return   
 */
void  IMU2_WriteRegister(uint8 SlaveID, uint8 reg, uint8 Data)
{
    IIC2_start();
    Write_IIC2_Byte(SlaveID << 1); //д���豸ID��д�ź�
    Write_IIC2_Byte(reg);
    Write_IIC2_Byte(Data);
    IIC2_stop();
}

/**
 * @brief    IIC������
 * @param    SlaveID �ӻ���ַ
 * @param    reg     �Ĵ�����ַ
 * @return   ����������
 */
uint8 IMU2_ReadRegister(uint8 SlaveID, uint8 reg)
{
    uint8 ret;
    
    IIC2_start();
    Write_IIC2_Byte(SlaveID << 1); //д���豸ID��д�ź�
    Write_IIC2_Byte(reg);
    IIC2_start();
    Write_IIC2_Byte((SlaveID << 1) + 1); //д���豸ID�����ź�
    ret = IIC2_read_byte();
    IIC2_stop();

    return ret;
}
