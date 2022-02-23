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
	delay_init();	    	 //延时函数初始?
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2	  
	usart2_config(115200);
	usart2_DMA_config(DMA1_Channel6,(u32)rxbuf,(u32)&USART2->DR,50);
	usart2_DMA_enable(DMA1_Channel6,50);
	CH455_Key_Init();

	
	all_pid_init();
	InitGraph(); 
	
	Init_2100_8700();
	
	Encoder_Init_TIM2();
	Encoder_Init_TIM4();
	TIM3_Int_Init(999,71);    //1ms需要1000次计数   中断频率=72M/72/1000=1000hz,
	
	TIM1_PWM_Init(999,4);
	
	menuinit();
	track_err=0;
	car_speed=80;
}

unsigned char start_flag=0;
unsigned char get_num_flag=0;
unsigned char num=0;

unsigned char track_12_flag=0;

char dir_buf[5];

void check_huowu(void)
{
	
	if(PBin(3)==1&&num==0)
	{
		//高电平左拐
		if(PBin(4)==1)
		{
			num=1;
			track_12_flag=1;
		}
		else
		{
			num=2;
			track_12_flag=1;
		}
		get_num_flag=0;
	}
	
	if(start_flag==0&&get_num_flag==0)
	{
		//检测openmv数字
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
//			else if(num==3)
//			{
//				dir_buf[0]='N';
//				dir_buf[1]='L';
//				dir_buf[2]=' ';
//			}
//			else if(num==4)
//			{
//				dir_buf[0]='N';
//				dir_buf[1]='R';
//				dir_buf[2]=' ';
//			}
//			else if(num==5)
//			{
//				dir_buf[0]='N';
//				dir_buf[1]='N';
//				dir_buf[2]='R';
//				dir_buf[3]='R';
//				dir_buf[4]=' ';
//			}
//			else if(num==6)
//			{
//				dir_buf[0]='N';
//				dir_buf[1]='N';
//				dir_buf[2]='L';
//				dir_buf[3]='L';
//				dir_buf[4]=' ';
//			}

			get_num_flag=1;
		}
	}
	
	if(PCin(15)==0&&start_flag==0)  //货物装上
	{
		delay_ms(500);
		start_flag=1;
	}

}


extern char send_flag;

void invert_pin_updata()
{
	if(send_flag)
	{
		static millis8_t lastUpdate;

		// Update every 20ms
		millis8_t now = millis();
		if ((millis8_t)(now - lastUpdate) >= 1500)
		{
			lastUpdate = now;

			PBout(5)=!PBout(5)	;
		}
	}
}

void c_loop()
{
	
	
	invert_pin_updata();
	check_huowu();
	buttons_updata();
	
}






	


int main(void)
{
	
	

	c_setup();
	
	while (1)
	{
		
	
		c_loop();
		
	}
}














