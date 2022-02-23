#include "func.h"
#include "key.h"
#include "gui.h"
#include "menu.h"
#include "delay.h"
#include "draw_api.h"
#include "stdio.h"
#include "string.h"
#include "pwm.h"

bool paraming = false;


float my_atof(const char* str)
{
    float s = 0.0;
    float d = 10.0;
    float flag = 1.0;

    if(*str == '-') {
        flag = -1.0 ;
        str ++;
    }
    while(*str >= '0' && *str <= '9' && *str != '.') {
        s = s * 10.0 + *str - '0';
        str ++;
    }
    if(*str == '.')str ++;
    while(*str >= '0' && *str <= '9') {
        s = s + (*str - '0') / d;
        d *= 10.0;
        str++;
    }
    return (s * flag);
}

///*demo!!!!!*/
//void set_mp()
//{
//	char temp_str[10],i=0;
//	uint8_t right_press_times = 0;
//	paraming = true;
//	
//	aera_blank();
//	gui_print_s(0,LINE(1),"recent value:");
//	gui_print_f(0,LINE(2),mp);
//	gui_print_s(0,LINE(3),"press # to input.");
//	gui_print_s(0,LINE(4),"press * to return.");
//	
//	
//	//清除上一次残留键值
//	key_infor.key_value = 0xff;
////	SHIFT_ON = true;
//	while (paraming)
//	{
//		if (key_infor.key_state==1)
//		{
//			SCAN_ENABLE = 0;//在处理完本次按键的值之前，先禁止按键检测
//			switch(key_infor.key_value)
//			{
//				gui_print_d(0,LINE(9),key_infor.key_value);
//				case ZERO:   temp_str[i++] = '0';temp_str[i+1] = '\0';gui_print_s(0,LINE(6),temp_str);break;
//				case ONE:    temp_str[i++] = '1';temp_str[i+1] = '\0';gui_print_s(0,LINE(6),temp_str);break;
//				case TWO:    temp_str[i++] = '2';temp_str[i+1] = '\0';gui_print_s(0,LINE(6),temp_str);break;
//				case THREE:  temp_str[i++] = '3';temp_str[i+1] = '\0';gui_print_s(0,LINE(6),temp_str);break;
//				case FOUR:   temp_str[i++] = '4';temp_str[i+1] = '\0';gui_print_s(0,LINE(6),temp_str);break;
//				case FIVE:   temp_str[i++] = '5';temp_str[i+1] = '\0';gui_print_s(0,LINE(6),temp_str);break;
//				case SIX:    temp_str[i++] = '6';temp_str[i+1] = '\0';gui_print_s(0,LINE(6),temp_str);break;
//				case SEVEN:  temp_str[i++] = '7';temp_str[i+1] = '\0';gui_print_s(0,LINE(6),temp_str);break;
//				case EIGHT:  temp_str[i++] = '8';temp_str[i+1] = '\0';gui_print_s(0,LINE(6),temp_str);break;
//				case NINE:   temp_str[i++] = '9';temp_str[i+1] = '\0';gui_print_s(0,LINE(6),temp_str);break;
//				case POINT:  temp_str[i++] = '.';temp_str[i+1] = '\0';gui_print_s(0,LINE(6),temp_str);break;
//				case RIGHT:  right_press_times+=1;
//				             if(right_press_times>=2){paraming = false;mp = my_atof((const char*)temp_str);SHIFT_ON = false;change_param(4,mp);gui_print_s(0,LINE(7),"put in SD ok!");systick_delay_ms(1000);break;}
//					         else {SHIFT_ON = true;gui_print_s(0,LINE(5),"Input please:");break;}
//				case LEFT :  paraming = false;gui_print_s(0,LINE(5),"warning:no change!");SHIFT_ON = false;systick_delay_ms(1000);break;;
//				             //退出的时候才关闭数字键
//				default : break;
//			}
//			SCAN_ENABLE = 1;//处理完本次键值，就能进行下次读取了
//			key_infor.key_value = 0xff;
//			key_infor.key_state=0;
//		}
//	}
//	showmenu();
//}
/*
void demo(void)
{
    paraming = true;
    aera_blank();
    //清除上一次残留键值
    key_infor.key_value = 0xff;
    while (paraming)
    {
        //
        if (key_infor.key_state==1)
        {
            SCAN_ENABLE = 0;//在处理完本次按键的值之前，先禁止按键检测
            switch(key_infor.key_value)
            {
                case LEFT : paraming = false;break;//退出的时候才关闭数字键

                default : break;
            }
            SCAN_ENABLE = 1;//处理完本次键值，就能进行下次读取了
            key_infor.key_value = 0xff;
            key_infor.key_state=0;
        }
    }
    showmenu();
}
*/
/*demo!!!!!*/
extern void check_huowu(void);
extern unsigned char num;
void show_data(void)
{
	paraming = true;
	aera_blank();
	//SetFontSize(1);
	//清除上一次残留键值
	key_infor.key_value = 0xff;
	while (paraming)
	{
		check_huowu();
		gui_print_s(0,LINE(1),"X:");
		gui_print_s(0,LINE(2),"W:");
		gui_print_d(32,LINE(1),X,3);
		gui_print_d(32,LINE(2),W,3);
		
		
		
		gui_print_s(64,LINE(1),"actual");
		gui_print_f(96,LINE(1),actual_value);
		
		gui_print_s(64,LINE(2),"err");
		gui_print_f(96,LINE(2),track_err);
		
		deal_buff[12]='\0';
		gui_print_s(0,LINE(3),deal_buff);
		
		gui_print_s(0,LINE(6),"num");
		gui_print_d(32,LINE(6),cnt,3);
		
		gui_print_s(0,LINE(7),"num");
		gui_print_d(32,LINE(7),num,3);
		gui_print_f(0,LINE(8),yaw);
	
		
		if(key_infor.key_backup!=key_infor.key_state)
		{
			if (key_infor.key_state==1)
			{
				switch(key_infor.key_value)
				{
					case RIGHT:break;
					case LEFT:
						
						paraming = false;
						SHIFT_ON = false;
						break;
					default : break;
				}
				key_infor.key_value = 0xff;
			}
			key_infor.key_backup=key_infor.key_state;
		}
		UpdateScreen();
	}
	showmenu();
}
void set_motor_speed(void)
{
	char temp_str[10]={'\0'},i=0;
	uint8_t right_press_times = 0;
	paraming = true;
	
	aera_blank();
	
	SHIFT_ON = true;
	//SetFontSize(1);
	//清除上一次残留键值
	key_infor.key_value = 0xff;
	while (paraming)
	{
		
		
		gui_print_s(0,LINE(1),"set_speed:");
	gui_print_d(64,LINE(1),(int)motorL.expect,3);
	
		gui_print_s(0,LINE(3),"Motor1.out:");
		gui_print_s(0,LINE(4),"Motor2.out:");
	gui_print_d(64,LINE(3),(int)Motor1.out,3);
	gui_print_d(64,LINE(4),(int)Motor2.out,3);
		gui_print_s(0,LINE(5),"Encoder_L:");
		gui_print_s(0,LINE(6),"Encoder_R:");
	gui_print_d(64,LINE(5),(int)Encoder_L,3);
	gui_print_d(64,LINE(6),(int)Encoder_R,3);
		
		if(key_infor.key_backup!=key_infor.key_state)
		{
			if (key_infor.key_state==1)
			{
				
			switch(key_infor.key_value)
			{
				case ZERO:   temp_str[i++] = '0';temp_str[i+1] = '\0';gui_print_s(0,LINE(2),temp_str);break;
				case ONE:    temp_str[i++] = '1';temp_str[i+1] = '\0';gui_print_s(0,LINE(2),temp_str);break;
				case TWO:    temp_str[i++] = '2';temp_str[i+1] = '\0';gui_print_s(0,LINE(2),temp_str);break;
				case THREE:  temp_str[i++] = '3';temp_str[i+1] = '\0';gui_print_s(0,LINE(2),temp_str);break;
				case FOUR:   temp_str[i++] = '4';temp_str[i+1] = '\0';gui_print_s(0,LINE(2),temp_str);break;
				case FIVE:   temp_str[i++] = '5';temp_str[i+1] = '\0';gui_print_s(0,LINE(2),temp_str);break;
				case SIX:    temp_str[i++] = '6';temp_str[i+1] = '\0';gui_print_s(0,LINE(2),temp_str);break;
				case SEVEN:  temp_str[i++] = '7';temp_str[i+1] = '\0';gui_print_s(0,LINE(2),temp_str);break;
				case EIGHT:  temp_str[i++] = '8';temp_str[i+1] = '\0';gui_print_s(0,LINE(2),temp_str);break;
				case NINE:   temp_str[i++] = '9';temp_str[i+1] = '\0';gui_print_s(0,LINE(2),temp_str);break;
				case POINT:  temp_str[i++] = '.';temp_str[i+1] = '\0';gui_print_s(0,LINE(2),temp_str);break;
				case RIGHT:  right_press_times+=1;
				             if(right_press_times>=1)
							{
								car_speed = my_atof((const char*)temp_str);
								SHIFT_ON = false;
								paraming = false;
								right_press_times=0;
								delay_ms(1000);
								break;
							}
					default : break;
			}
			key_infor.key_value = 0xff;
			
			}
			key_infor.key_backup=key_infor.key_state;
		}
		UpdateScreen();
	}
	showmenu();
}

extern unsigned char get_num_flag;

void set_openmv_num()
{
	char temp_str[10]={'\0'},i=0;
	uint8_t right_press_times = 0;
	paraming = true;
	
	aera_blank();
	
	SHIFT_ON = true;
	//SetFontSize(1);
	//清除上一次残留键值
	key_infor.key_value = 0xff;
	while (paraming)
	{
		
		
		gui_print_s(0,LINE(1),"set_num:");

		
		if(key_infor.key_backup!=key_infor.key_state)
		{
			if (key_infor.key_state==1)
			{
				
			switch(key_infor.key_value)
			{
				case ZERO:   temp_str[i++] = '0';temp_str[i+1] = '\0';gui_print_s(0,LINE(2),temp_str);break;
				case ONE:    temp_str[i++] = '1';temp_str[i+1] = '\0';gui_print_s(0,LINE(2),temp_str);break;
				case TWO:    temp_str[i++] = '2';temp_str[i+1] = '\0';gui_print_s(0,LINE(2),temp_str);break;
				case THREE:  temp_str[i++] = '3';temp_str[i+1] = '\0';gui_print_s(0,LINE(2),temp_str);break;
				case FOUR:   temp_str[i++] = '4';temp_str[i+1] = '\0';gui_print_s(0,LINE(2),temp_str);break;
				case FIVE:   temp_str[i++] = '5';temp_str[i+1] = '\0';gui_print_s(0,LINE(2),temp_str);break;
				case SIX:    temp_str[i++] = '6';temp_str[i+1] = '\0';gui_print_s(0,LINE(2),temp_str);break;
				case SEVEN:  temp_str[i++] = '7';temp_str[i+1] = '\0';gui_print_s(0,LINE(2),temp_str);break;
				case EIGHT:  temp_str[i++] = '8';temp_str[i+1] = '\0';gui_print_s(0,LINE(2),temp_str);break;
				case NINE:   temp_str[i++] = '9';temp_str[i+1] = '\0';gui_print_s(0,LINE(2),temp_str);break;
				case POINT:  temp_str[i++] = '.';temp_str[i+1] = '\0';gui_print_s(0,LINE(2),temp_str);break;
				case RIGHT:  right_press_times+=1;
				             if(right_press_times>=1)
							{
								
								get_num_flag=0;
								num = my_atof((const char*)temp_str);
								SHIFT_ON = false;
								paraming = false;
								right_press_times=0;
								delay_ms(1000);
								break;
							}
					default : break;
			}
			key_infor.key_value = 0xff;
			
			}
			key_infor.key_backup=key_infor.key_state;
		}
		UpdateScreen();
	}
	showmenu();
}

void hopper(void)
{
    aera_blank();
    //清除上一次残留键值
    key_infor.key_value = 0xff;

	

    showmenu();
}


void set_font(void)
{
	unsigned char i=_FontSize,backup=_FontSize;
	
	paraming = true;
	SetFontSize(1);
	
    aera_blank();
	gui_print_s(0,LINE(1),"recent size:");
	gui_print_d(0,LINE(2),i,1);
	gui_print_s(0,LINE(3),"press # to use ");
	gui_print_s(0,LINE(4),"press * to return");
	UpdateScreen();
    key_infor.key_value = 0xff;
    while (paraming)
    {
        //
		if(key_infor.key_backup!=key_infor.key_state)
		{
			if (key_infor.key_state==1)
			{
				switch(key_infor.key_value)
				{
					case RIGHT : 
						SetFontSize(2);
						gui_print_s(96,LINE(4),"ok");
						UpdateScreen();
						delay_ms(1000);
						SetFontSize(i);
						paraming = false;
						break;
					case UP:
						 ClearScreen();
						SetFontSize(++i);
						gui_print_s(0,LINE(1),"haha");
					UpdateScreen();
						break;
					case DOWN:
						 ClearScreen();
						SetFontSize(--i);
						gui_print_s(0,LINE(1),"haha");
					UpdateScreen();
						
						break;//退出的时候才关闭数字键
					case LEFT:
						paraming = false;
						SetFontSize(backup);
						gui_print_s(0,LINE(4),"no change!");
						UpdateScreen();
						SHIFT_ON = false;
						delay_ms(1000);
						break;

					default : break;
				}
				key_infor.key_value = 0xff;
			}
			key_infor.key_backup=key_infor.key_state;
		}
    }
    showmenu();
}


void author(void)
{
	unsigned char backup=_FontSize;
	paraming = true;
	SetFontSize(1);
    aera_blank();
	gui_print_s(0,LINE(1),"we");
	gui_print_s(0,LINE(4),"development date: ");
	gui_print_s(0,LINE(5),"          2021/5/1");
	gui_print_s(0,LINE(6),"software version:");
	gui_print_s(0,LINE(7),"          v1.00");
	UpdateScreen();
    key_infor.key_value = 0xff;
    while (paraming)
    {
		if(key_infor.key_backup!=key_infor.key_state)
		{
			if (key_infor.key_state==1)
			{
				switch(key_infor.key_value)
				{
					case LEFT:
						paraming = false;
						SHIFT_ON = false;
						break;
					default : break;
				}
				key_infor.key_value = 0xff;
			}
			key_infor.key_backup=key_infor.key_state;
		}
    }
	SetFontSize(backup);
    showmenu();
}
