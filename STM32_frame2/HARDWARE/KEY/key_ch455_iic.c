#include "key_ch455_iic.h"
#include "stdbool.h"
#include "usart.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "myinclude.h"

#if USE_CH455_KEY
/*shift switch flag:
0x00:function,such as up/down/ok/left and so on;
0xff:numbers
defult:function*/


//ע��GPIO_num�����ֲ��Ƕ���
void gpio_dir(GPIO_TypeDef* GPIOx, uint16_t GPIO_num , unsigned char Dir)
{
	if(Dir==GPI) //input
	{
		if(GPIO_num<=7)
		{
			switch(GPIO_num)
			{
				case 0 :GPIOx->CRL&=0XFFFFFFF0;break;
				case 1 :GPIOx->CRL&=0XFFFFFF0F;break;
				case 2 :GPIOx->CRL&=0XFFFFF0FF;break;
				case 3 :GPIOx->CRL&=0XFFFF0FFF;break;
				case 4 :GPIOx->CRL&=0XFFF0FFFF;break;
				case 5 :GPIOx->CRL&=0XFF0FFFFF;break;
				case 6 :GPIOx->CRL&=0XF0FFFFFF;break;
				case 7 :GPIOx->CRL&=0X0FFFFFFF;break;
			}
			GPIOx->CRL|=(u32)8<<(4*GPIO_num);
		}
		else
		{
			switch(GPIO_num-8)
			{
				case 0 :GPIOx->CRH&=0XFFFFFFF0;break;
				case 1 :GPIOx->CRH&=0XFFFFFF0F;break;
				case 2 :GPIOx->CRH&=0XFFFFF0FF;break;
				case 3 :GPIOx->CRH&=0XFFFF0FFF;break;
				case 4 :GPIOx->CRH&=0XFFF0FFFF;break;
				case 5 :GPIOx->CRH&=0XFF0FFFFF;break;
				case 6 :GPIOx->CRH&=0XF0FFFFFF;break;
				case 7 :GPIOx->CRH&=0X0FFFFFFF;break;
			}
			GPIOx->CRH|=(u32)8<<(4*(GPIO_num-8));
		}
	}
	else if(Dir==GPO)  //output
    {
		if(GPIO_num<=7)
		{
			switch(GPIO_num)
			{
				case 0 :GPIOx->CRL&=0XFFFFFFF0;break;
				case 1 :GPIOx->CRL&=0XFFFFFF0F;break;
				case 2 :GPIOx->CRL&=0XFFFFF0FF;break;
				case 3 :GPIOx->CRL&=0XFFFF0FFF;break;
				case 4 :GPIOx->CRL&=0XFFF0FFFF;break;
				case 5 :GPIOx->CRL&=0XFF0FFFFF;break;
				case 6 :GPIOx->CRL&=0XF0FFFFFF;break;
				case 7 :GPIOx->CRL&=0X0FFFFFFF;break;
			}
			GPIOx->CRL|=(u32)3<<(4*GPIO_num);
		}
		else
		{
			switch(GPIO_num-8)
			{
				case 0 :GPIOx->CRH&=0XFFFFFFF0;break;
				case 1 :GPIOx->CRH&=0XFFFFFF0F;break;
				case 2 :GPIOx->CRH&=0XFFFFF0FF;break;
				case 3 :GPIOx->CRH&=0XFFFF0FFF;break;
				case 4 :GPIOx->CRH&=0XFFF0FFFF;break;
				case 5 :GPIOx->CRH&=0XFF0FFFFF;break;
				case 6 :GPIOx->CRH&=0XF0FFFFFF;break;
				case 7 :GPIOx->CRH&=0X0FFFFFFF;break;
			}
			GPIOx->CRH|=(u32)3<<(4*(GPIO_num-8));
		}
	}

}


void ch455_iic_delay(int t)
{
    int i = t;
    while(i--);
}

void ch455_iic_start(void)
{
    CH455_SDA_H;
    CH455_SCL_H;
    CH455_IIC_DELAY;
    CH455_SDA_L;
    CH455_IIC_DELAY;
    CH455_SCL_L;
}
void ch455_iic_stop(void)
{
    CH455_SCL_L;
    CH455_SDA_L;
    CH455_IIC_DELAY;
    CH455_SCL_H;
    CH455_IIC_DELAY;
    CH455_SDA_H;
    CH455_IIC_DELAY;
}
void ch455_iic_write_byte(vuint8 byte)
{
    vuint8 mask;
    for (mask=0x80; mask!=0; mask>>=1) //�Ӹ�λ����λ���ν���
    {
        if ((mask&byte) == 0)  //��λ��ֵ�����SDA��
            CH455_SDA_L;
        else
            CH455_SDA_H;
        CH455_IIC_DELAY;
        CH455_SCL_H;          //����SCL
        CH455_IIC_DELAY;
        CH455_SCL_L;      //������SCL�����һ��λ����
    }
    CH455_SDA_H;   //8λ���ݷ�����������ͷ�SDA���Լ��ӻ�Ӧ��
    CH455_IIC_DELAY;
    CH455_SCL_H;  //����SCL

    //ռλ���˴����Զ�ȡ�ӻ���Ӧ

    CH455_IIC_DELAY;
    CH455_SCL_L;
}
vuint8 ch455_read_byte(void)
{
    vuint8 i;
    vuint8 c = 0;
    CH455_SDA_D_IN;//*���ʹ��
    CH455_SDA_H;  //����ȷ�������ͷ�SDA

    for (i=0;i<8;i++)
    {
        CH455_IIC_DELAY;
        CH455_SCL_H;      //����SCL
        c <<= 1;
        if (CH455_SDA_IN) 
        {
            c += 1;
        }
        CH455_IIC_DELAY;
        CH455_SCL_L;

    }
    CH455_SDA_D_OUT;//*���ʹ��

    CH455_SDA_H;  //8λ���ݷ����������SDA�����ͷ�Ӧ���ź�
    CH455_IIC_DELAY;
    CH455_SCL_H;   //����SCL
    CH455_IIC_DELAY;
    CH455_SCL_L;
    return c;
}
void ch455_write(vuint8 byte_1,vuint8 byte_2)
{
    ch455_iic_start();
    ch455_iic_write_byte(byte_1);
    ch455_iic_write_byte(byte_2);
    ch455_iic_stop();
}
vuint8 ch455_read_key_value(void)
{
    vuint8 key_value;
    ch455_iic_start();
    ch455_iic_write_byte(0x4f);
    key_value = ch455_read_byte();
    ch455_iic_stop();
	//printf("%x",key_value);
    return (key_value);
}


void gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//ʹ��GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13); 	//PB6,PB7 �����
}

void EXIT_Init(void)
{
	  EXTI_InitTypeDef EXTI_InitStructure;
 	  NVIC_InitTypeDef NVIC_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�ⲿ�жϣ���Ҫʹ��AFIOʱ��
	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//ʹ��GPIOBʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;   //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_14); 	//

	 //GPIOB.14 �ж����Լ��жϳ�ʼ������
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line14;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//�����ȼ�1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 
	
}

void CH455_Key_Init(void)
{
    gpio_init();
	
    EXIT_Init();

    delay_ms(1);
	CH455_SCL_L;
	CH455_SDA_L;
    delay_ms(1);
    CH455_SCL_H;
	CH455_SDA_H;
    delay_ms(1);

    ch455_write(0x48,0x4d);
    ch455_read_key_value();
}





unsigned char key_info_set(void)
{
    vuint8 temp_value = 0xff;

    temp_value= ch455_read_key_value();
	key_infor.key_value = 0xff;
	
/*key value
|------------------------|
|                        |
|         ips            |
|                        |
|                        |
|------------------------|
[no use] [  up  ] [no use]
[no use] [no use] [no use]
[no use] [no use] [no use]
[left  ] [down  ] [RIGHT ]
*/
	
//key_infor.key_backup = 0;   //ɨ����ж�ģʽ�����жϷ����������һ�������
if(if_use_ch455==1)
{
     //printf("%x",temp_value);
        if (SHIFT_ON==false)
        {
            switch(temp_value)
            {
                case 0x55: break;
                case 0x56: key_infor.key_state = 1;key_infor.key_value = UP; break;
                case 0x57: break;
                case 0x5d: break;
                case 0x5e: break;
                case 0x5f: break;
                case 0x65: break;
                case 0x66: key_infor.key_state = 1;key_infor.key_value = DOWN; break;
                case 0x67: break;
                case 0x6d: key_infor.key_state = 1;key_infor.key_value = LEFT; break;
                case 0x6e: key_infor.key_state = 1;key_infor.key_value = DOWN; break;
                case 0x6f: key_infor.key_state = 1;key_infor.key_value = RIGHT; break;
				default :key_infor.key_state = 0;break; //ɨ��ģʽ
            }
        }
        else if (SHIFT_ON==true)//���ּ���������������Ļص�������
        {
            switch(temp_value)
            {
                case 0x55: key_infor.key_state = 1;key_infor.key_value = ONE;break;
                case 0x56: key_infor.key_state = 1;key_infor.key_value = TWO; break;
                case 0x57: key_infor.key_state = 1;key_infor.key_value = THREE;break;
                case 0x5d: key_infor.key_state = 1;key_infor.key_value = FOUR; break;
                case 0x5e: key_infor.key_state = 1;key_infor.key_value = FIVE; break;
                case 0x5f: key_infor.key_state = 1;key_infor.key_value = SIX; break;
                case 0x65: key_infor.key_state = 1;key_infor.key_value = SEVEN;break;
                case 0x66: key_infor.key_state = 1;key_infor.key_value = EIGHT; break;
                case 0x67: key_infor.key_state = 1;key_infor.key_value = NINE;break;
                case 0x6d: key_infor.key_state = 1;key_infor.key_value = POINT; break;
                case 0x6e: key_infor.key_state = 1;key_infor.key_value = ZERO; break;
                case 0x6f: key_infor.key_state = 1;key_infor.key_value = RIGHT; break;
                default :key_infor.key_state = 0;break;
            }
        }
        else
        {

        }
}

else
{
	   if (SHIFT_ON==0)
        {
            switch(temp_value)
            {

                case '2': key_infor.key_state = 1;key_infor.key_value = UP; break;
                case '8': key_infor.key_state = 1;key_infor.key_value = DOWN; break;
                case 'u': key_infor.key_state = 1;key_infor.key_value = LEFT; break;
                case 'l': key_infor.key_state = 1;key_infor.key_value = DOWN; break;
                case 'd': key_infor.key_state = 1;key_infor.key_value = RIGHT; break;
        default :key_infor.key_state = 0;break;
            }
        }
        else if (SHIFT_ON==1)//���ּ���������������Ļص�������
        {
            switch(temp_value)
            {
                case '1': key_infor.key_state = 1;key_infor.key_value = ONE;break;
                case '2': key_infor.key_state = 1;key_infor.key_value = TWO; break;
                case '3': key_infor.key_state = 1;key_infor.key_value = THREE;break;
                case '4': key_infor.key_state = 1;key_infor.key_value = FOUR; break;
                case '5': key_infor.key_state = 1;key_infor.key_value = FIVE; break;
                case '6': key_infor.key_state = 1;key_infor.key_value = SIX; break;
                case '7': key_infor.key_state = 1;key_infor.key_value = SEVEN;break;
                case '8': key_infor.key_state = 1;key_infor.key_value = EIGHT; break;
                case '9': key_infor.key_state = 1;key_infor.key_value = NINE;break;
                case '.': key_infor.key_state = 1;key_infor.key_value = POINT; break;
                case '0': key_infor.key_state = 1;key_infor.key_value = ZERO; break;
                case 'd': key_infor.key_state = 1;key_infor.key_value = RIGHT; break;
                default :key_infor.key_state = 0; break;
            }
        }
        else
        {
			
        }
}
return key_infor.key_value;
}



void EXTI15_10_IRQHandler(void)
{			
	

	 key_info_set();
	 key_infor.key_backup = 0;  //ɨ��ģʽ��Ϊ�ж�Ҫ�����һ���仰
 	 EXTI_ClearITPendingBit(EXTI_Line14);    //���LINE5�ϵ��жϱ�־λ  
}



#endif


