#include "controller.h"
#include "pid.h"
#include "myinclude.h"

_OUT_Motor Motor1 = {0};
_OUT_Motor Motor2 = {0};

float car_speed=0;
float set_speed=0;
float track_err=0;



void motor_control(float _err)
{
	Encoder_L=-Read_Encoder(2);//左电机编码器值
	Encoder_R=Read_Encoder(4);//右电机编码器值
	
	
	motorL.expect = set_speed+_err;
    motorL.feedback = Encoder_L;                   
    pid_controller(&motorL);                              
	
	motorR.expect = set_speed-_err;                 
    motorR.feedback = Encoder_R;                    
    pid_controller(&motorR);                             
	
	Motor1.out = (int)motorL.out;  //限幅
	Motor2.out = (int)motorR.out;
	
	if(Motor1.out>=0)
	{
		TIM_SetCompare4(TIM1,Motor1.out);
		TIM_SetCompare3(TIM1,0);
	}
	else
	{
		TIM_SetCompare4(TIM1,0);
		TIM_SetCompare3(TIM1,-Motor1.out);
	}
	if(Motor2.out>=0)
	{
		TIM_SetCompare1(TIM1,Motor2.out);
		TIM_SetCompare2(TIM1,0);
	}
	else
	{
		TIM_SetCompare1(TIM1,0);
		TIM_SetCompare2(TIM1,-Motor2.out);
	}              
}


unsigned char check_num;

unsigned char shizi_flag=0;
char deal_buff[13]={'X','1','6','0','Y','1','6','0','W','0','0','0','\0'}; 
short X;
short W;

char X_buff[5];

float actual_value;

float limt(float in,float max)
{
	if(in >  max)   return max;
    if(in < -max)   return -max;
	return in;
}


extern char dir_buf[5];
extern unsigned char start_flag;
char cnt=0;
char stop_flag=0;
char turn_flag=0;
unsigned char back_flag=0;

char yaw_keep;
float yaw_last;


char num_get_flag=0;

extern unsigned char track_12_flag;

void read_num()
{
	if(track_12_flag==0)
	{
		if(PAin(12)==1)
		{
			//高电平左拐
			//这个是汽车二，方向已经颠倒
			if(PAin(15)==1)
			{
				num_get_flag = 'L';
			}
			else
			{
				num_get_flag = 'R';
			}
		}
	}
}

void shizi_deal_L_R()
{
	if(track_12_flag==0)
	{
		if(num_get_flag != 0)
		{
			dir_buf[cnt] = num_get_flag;
		}
		else
		{
			dir_buf[cnt] = 'N';
		}
		num_get_flag = 0;
	}
}

char stop_deal_flag=0;
char car1_ok=0;

extern char return_flag;
char ooook_flag=0;


void track_deal(void)
{
	if(usart2IdleItFlag)
	{
		memcpy(&deal_buff,rxbuf,12);
		usart2_DMA_enable(DMA1_Channel6,50);//ê1?ü??ò?′?DMA
		usart2IdleItFlag = 0;//一定不要忘记将标志位清零。
		for(int i=0;i<12;i++)
		{
			if(deal_buff[i]==' ')
			{
				deal_buff[i]='0';
			}
		}
	}
	
	//忽略这次读取，因为电平延时的问题

	{
		read_num();
	}
	
	X=(deal_buff[1]-'0')*100+(deal_buff[2]-'0')*10+(deal_buff[3]-'0');
	W=(deal_buff[9]-'0')*100+(deal_buff[10]-'0')*10+(deal_buff[11]-'0');
	
	

	track_err=((float)X-160.0)*0.2; //循迹误差
	
	track_err=limt(track_err,30);
	
	//识别十字
	if(W>115&&!shizi_flag&&!turn_flag&&start_flag==1&&stop_deal_flag==0)
	{

		shizi_flag=125;
		
		

		ang_gyro=0.0;  //陀螺仪积分清零
		yaw=0;
		
		if(back_flag==0)  //只有前进判断
		{
			if(cnt==1)
				shizi_deal_L_R();
			
			

			dir_buf[0]='R';
			//中道判断
			if(cnt==1&&dir_buf[cnt]!='N')  //说明识别到了
			{
				dir_buf[cnt]='R';
				dir_buf[cnt+1]=' ';
				ooook_flag=1;
			}
			if(cnt==1&&dir_buf[cnt]=='N')
			{
				dir_buf[cnt]='L';
				dir_buf[cnt+1]=' ';
				ooook_flag=1;
			}
			
		
			
		}

		if(dir_buf[cnt+1]==' '&&back_flag==0)
		{
			stop_flag=1;
			if(dir_buf[cnt]=='R')
			{
				yaw_last=-90;
			}
			else if(dir_buf[cnt]=='L')
			{
				yaw_last=90;
			}
		}
		

	}
	
	
	
	if(shizi_flag==125)
	{
		static u16 time_dealy=0;
		
		if(time_dealy++>10)
		{
			time_dealy=0;
			shizi_flag=1;
		}
	}
	
	if(stop_flag==1||ooook_flag==1)  //等待时间停车
	{
		static u16 delayms=0;
		
		if(!shizi_flag)
		{
			actual_value=0;
		}
		if(delayms++>110)
		{
			
			
			yaw_keep=1;
			delayms=0;
			stop_flag=2;  //停车完毕
			shizi_flag=0;
		}
	}
	
	
	
	
	if(stop_flag==2)  //停车检测
	{
		if(ooook_flag==1)
			PAout(2)=0;  //红灯亮
		if(ooook_flag==0)  //检测到货物离开
		{
			static u16 delayms;
			if(delayms++>30)
			{
				delayms=0;
				
			
				stop_flag=0;
				turn_flag=1;
				ang_gyro=0.0;  //陀螺仪积分清零
				yaw=0;
				yaw_keep=0;  //开始转动，关闭保持

			}
		}
	}
	//最后停车
	else if(stop_flag==3)
	{
		static u16 delayms;
		if(delayms++>110)
		{
			delayms=0;
			stop_flag=4;  //最后停车完成,此时车已经停
			PCout(14)=0;  //绿灯亮
		}
	}
	
	//旋转180
	if(turn_flag==1)
	{
		actual_value = -car_speed*0.5;  //陀螺仪误差寻迹 //陀螺仪误差寻迹
		if(yaw>170&&yaw<190)
		{
			actual_value=0;
		}
		if (
				yaw>170
				)
		{
			turn_flag=0;//转弯完成
			return_flag=1;
			stop_flag=6;
			PBout(8)=0;   //黄灯亮
			
		}
	}
	
	if(stop_flag==6)
	{
		yaw_keep=1;
		yaw_last=170;
		if(car1_ok==1)
		{
			PBout(8)=1;   //黄灯灭
			stop_flag=0;
			cnt=0;
			dir_buf[0]='R';
			dir_buf[1]='R';
			yaw_keep=0;
		}
	}
	

	
	if(shizi_flag==1)
	{
		
		
		if(dir_buf[cnt]=='L')
		{
			actual_value = -car_speed*0.4;  //陀螺仪误差寻迹 //陀螺仪误差寻迹
			if(yaw<100&&yaw>60)
			{
				actual_value=-0.5 * ( 90 - yaw);
			}
			if (
					yaw>70
					)
			{
				shizi_flag=0;
				cnt++;
			}
		}
		else if(dir_buf[cnt]=='R')
		{
		
			actual_value = car_speed*0.4;  //陀螺仪误差寻迹 //陀螺仪误差寻迹
			if(yaw<-60&&yaw>-100)
			{
				actual_value=-0.5 * ( -90 - yaw);
			}
			if (
					yaw<-70
					)
			{
				shizi_flag=0;
				cnt++;
			}
		}
		else if(dir_buf[cnt]=='N')
		{
			static u16 delayms;
			if(delayms++>30)
			{
				delayms=0;
				shizi_flag=0;
				cnt++;
			}
		}

	}
	else
	{
		if(stop_deal_flag==0&&turn_flag==0&&shizi_flag==0)
		{
			actual_value=track_err;
		}
		if(yaw_keep==1)
		{
			actual_value = -2 * ( yaw_last - yaw);
		}
	}

	
}


void track_deal2(void)
{
	if(usart2IdleItFlag)
	{
		memcpy(&deal_buff,rxbuf,12);
		usart2_DMA_enable(DMA1_Channel6,50);//ê1?ü??ò?′?DMA
		usart2IdleItFlag = 0;//一定不要忘记将标志位清零。
		for(int i=0;i<12;i++)
		{
			if(deal_buff[i]==' ')
			{
				deal_buff[i]='0';
			}
		}
	}
	
	X=(deal_buff[1]-'0')*100+(deal_buff[2]-'0')*10+(deal_buff[3]-'0');
	W=(deal_buff[9]-'0')*100+(deal_buff[10]-'0')*10+(deal_buff[11]-'0');
	
	
	track_err=((float)X-160.0)*0.2; //循迹误差
	
	track_err=limt(track_err,30);
	
	if(W>115&&!shizi_flag&&!turn_flag&&start_flag==1&&stop_deal_flag==0)
	{
		shizi_flag=1;
		stop_flag=1;
	}
	
	
	
	if(stop_flag==1)  //等待时间停车
	{
		static u16 delayms=0;
		
		if(!shizi_flag)
		{
			actual_value=0;
		}
		if(delayms++>110)
		{
			yaw_keep=1;
			delayms=0;
			stop_flag=2;  //停车完毕
			shizi_flag=0;
		}
	}
	if(stop_flag==2)  //停车检测
	{
		static u16 delayms=0;
		if(delayms++>60)
		{
			delayms=0;
			PAout(2)=0;  //红灯亮
		}
	}
	
	if(stop_flag==6)
	{
		yaw_keep=1;
		yaw_last=170;
		if(car1_ok==1)
		{
			stop_flag=0;
			yaw_keep=0;
		}
	}
	
	if(shizi_flag==1)
	{
		static u16 delayms;
		actual_value=0;
		if(delayms++>30)
		{
			delayms=0;
			shizi_flag=0;
		}
	}
	else
	{
		if(stop_deal_flag==0)
		{
			actual_value=track_err;
		}
		if(yaw_keep==1)
		{
			actual_value = -2 * ( yaw_last - yaw);
		}
	}
	
	
	
}

extern unsigned char start_flag;

void motor_speed(void)
{
	if(start_flag==0)
	{
		num_get_flag=0;
		shizi_flag=0;
		actual_value=0;
		set_speed=0;
	}
	else if(turn_flag==1||stop_flag==2||stop_deal_flag==2||stop_deal_flag==3||stop_flag==4||stop_flag==6)
	{
		set_speed=0;
	}
	else if(shizi_flag&&dir_buf[cnt]!='N')
	{
		set_speed=car_speed*0.7;
	}
	else
	{
		
		set_speed=car_speed;
	}
	
	
	{
		static char ok=1;
		
		if(back_flag==1&&ok==1)
		{
			if(shizi_flag==2)  //遇到第一个十字时就取消减速
			{
				ok=0;
			}
			set_speed=50;
		}
	}
}

















//void _controller_perform(void)                                  //pid控制器运行
//{
//	
//	                   
//}


//关闭输出检测
//static uint8_t shut_down(float angle)
//{
////    uint8_t shut_flag = 0;
////    if(f_abs(angle)>20.0f)                                             
////    {
////        shut_flag = 1;                                              //关闭输出标志置位                 
////    }                                                            
//    return true;                                            
//}                  

//_JUMP_DEFINE Jump = {0};
//控制器pwm输出                                                  
void _controller_output(void)                                    
{


//    
//    if(Motor1.out>0)                                            //根据输出正负来判断方向
//        DIR_LOW;   
//    else 
//        DIR_HIGH;
//    
//    motor_pwm_out(0,int_abs(Motor1.out),0);                     //电机输出
} 





