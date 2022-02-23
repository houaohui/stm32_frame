#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h" 
#include "draw_api.h"
#include "test.h"
#include "key.h"
#include "menu.h"
#include "timer.h"
#include "pwm.h"
#include "key_ch455_iic.h"
#include "myinclude.h"







void c_setup()
{
	
	LED_Init();
	delay_init();	    	 //��ʱ������ʼ?
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2	  
	usart2_config(115200);
	usart2_DMA_config(DMA1_Channel6,(u32)rxbuf,(u32)&USART2->DR,50);
	usart2_DMA_enable(DMA1_Channel6,50);
	CH455_Key_Init();

	
	all_pid_init();
	InitGraph(); 
	
	Init_2100_8700();
	
	Encoder_Init_TIM2();
	Encoder_Init_TIM4();
	TIM3_Int_Init(999,71);    //1ms��Ҫ1000�μ���   �ж�Ƶ��=72M/72/1000=1000hz,
	
	TIM1_PWM_Init(999,4);
	
	menuinit();
	track_err=0;
	car_speed=60;
}

unsigned char start_flag=0;
unsigned char get_num_flag=0;
unsigned char num=2;

unsigned char track_12_flag=0;

char dir_buf[5];

char first_step=0;

void check_huowu(void)
{
	
//	if(PBin(3)==1&&num==0)
//	{
//		//�ߵ�ƽ���
//		if(PBin(4)==1)
//		{
//			num=1;
//			track_12_flag=1;
//		}
//		else
//		{
//			num=2;
//			track_12_flag=1;
//		}
//		get_num_flag=0;
//	}
	
	if(start_flag==0&&get_num_flag==0)
	{
		//���openmv����
		if(num)
		{
			if(num==1)
			{
				dir_buf[0]='L';
				dir_buf[1]=' ';
			}
			else if(num==2)
			{
				dir_buf[0]='R';
				dir_buf[1]=' ';
			}


			get_num_flag=1;
		}
	}
	
	if(PCin(15)==0&&start_flag==0)  //����װ��
	{
		delay_ms(500);
		start_flag=1;
	}

}

char pin_backup;
void c_loop()
{
	
	check_huowu();
	buttons_updata();
	
}





	
char return_flag=0;
extern char turn_flag;

extern char car1_ok;
int main(void)
{
	
	

	c_setup();
	
	
	delay_ms(200);
	
	pin_backup=PBin(5);
	while (1)
	{
		
		//������ŵ�ƽ״̬�仯
		if(PBin(5)!=pin_backup)
		{
			car1_ok=1;
			PBout(8)=1;   //�Ƶ���

		}
		pin_backup=PBin(5);
	
		c_loop();
		
	}
}














