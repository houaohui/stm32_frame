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


//注意GPIO_num是数字不是定义
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
    for (mask=0x80; mask!=0; mask>>=1) //从高位到低位依次进行
    {
        if ((mask&byte) == 0)  //该位的值输出到SDA上
            CH455_SDA_L;
        else
            CH455_SDA_H;
        CH455_IIC_DELAY;
        CH455_SCL_H;          //拉高SCL
        CH455_IIC_DELAY;
        CH455_SCL_L;      //再拉低SCL，完成一个位周期
    }
    CH455_SDA_H;   //8位数据发送完后，主机释放SDA，以检测从机应答
    CH455_IIC_DELAY;
    CH455_SCL_H;  //拉高SCL

    //占位：此处可以读取从机回应

    CH455_IIC_DELAY;
    CH455_SCL_L;
}
vuint8 ch455_read_byte(void)
{
    vuint8 i;
    vuint8 c = 0;
    CH455_SDA_D_IN;//*配对使用
    CH455_SDA_H;  //首先确保主机释放SDA

    for (i=0;i<8;i++)
    {
        CH455_IIC_DELAY;
        CH455_SCL_H;      //拉高SCL
        c <<= 1;
        if (CH455_SDA_IN) 
        {
            c += 1;
        }
        CH455_IIC_DELAY;
        CH455_SCL_L;

    }
    CH455_SDA_D_OUT;//*配对使用

    CH455_SDA_H;  //8位数据发送完后，拉高SDA，发送非应答信号
    CH455_IIC_DELAY;
    CH455_SCL_H;   //拉高SCL
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
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//使能GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13); 	//PB6,PB7 输出高
}

void EXIT_Init(void)
{
	  EXTI_InitTypeDef EXTI_InitStructure;
 	  NVIC_InitTypeDef NVIC_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//外部中断，需要使能AFIO时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//使能GPIOB时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;   //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_14); 	//

	 //GPIOB.14 中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line14;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
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
	
//key_infor.key_backup = 0;   //扫描改中断模式，在中断服务函数中添加一句就行了
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
				default :key_infor.key_state = 0;break; //扫描模式
            }
        }
        else if (SHIFT_ON==true)//数字键功能在输入参数的回调函数打开
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
        else if (SHIFT_ON==1)//数字键功能在输入参数的回调函数打开
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
	 key_infor.key_backup = 0;  //扫描模式改为中断要添加这一按句话
 	 EXTI_ClearITPendingBit(EXTI_Line14);    //清除LINE5上的中断标志位  
}



#endif


