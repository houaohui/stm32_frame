#include "menu.h"
#include "draw_api.h"
#include "func.h"
#include "string.h"
#include "key.h"
#include "gui.h"
/*��������Ҫ�õ���ͷ�ļ�*/
//#include "pid.h"
//#include "param.h"



menu_t menu;

menu_item_t* current_level_branchs[LEVEL_BRANCH_MAX];

void empty(void);
void menuback(void);

void aera_blank(void)
{
	/*wersion 1*/
    ClearScreen();
	
//	/*version 2*/
//	UG_FillFrame(0,0,239,LINE(8),GUI_main.back_color);
}
menu_item_t menu_items[] =
{
	
	{"/-->",empty,NULL,false},
	
	{"/-->/>0.show_data",show_data,NULL,false},


	{"/-->/>1.set_speed",set_motor_speed,NULL,false},
	
	{"/-->/>2.set num",set_openmv_num,NULL,false},

	{"/-->/>2.play game",empty,NULL,false},
	{"/-->/>2.play game/1.hopper!",hopper,NULL,false},
	{"/-->/>2.play game/2.snack  ",empty,NULL,false},
	
	{"/-->/>3.set font",set_font,NULL,false},
	
	{"/-->/>4.author",author,NULL,false}

};

void empty(void)
{
	  ;
}

void menu_param_init(void)
{
	vuint8 i;
	menu.max_deep = 0;       //����0�����ר����max_deep
	menu.current_deep = 1;   
	menu.items_amount = 0;   //����0�����ר����items_amount
	menu.current_level_items_amount = 1;     //��ʼ����ʱ�������ʾroot�������1
	menu.current_level_item_order = 1;       //ÿ�ζ��ǵ�һ���������ٱ�
	menu.deep_change_flag = 0;
	menu.fuc_run_falg = 0;
	menu.current_item_pointer = &menu_items[0];     //��ǰ�˵�������menu_items[0]
	//
	/*ע�⣺current_item_str��menu�����ԣ�item_str_pointer�ǲ˵��������*/
	/*�˴����ҽ�menu[0]���ַ�װ��current_item_str*/
	for (i=0;menu_items[0].item_str_pointer[i] != '\0';i++)
	{
		menu.current_item_str[i] = menu_items[0].item_str_pointer[i];
	}
	menu.current_item_str[i] = '\0';
}
void set_items_amount(void)
{
	menu.items_amount = sizeof(menu_items)/sizeof(menu_items[0]);
}
void set_max_deep(void)
{
	vuint8 i=0,j=0,count=0;
	
	for(i=0;i<menu.items_amount;i++) //��߿��������ڼ���item
	{
		count = 0;
		
		for (j=0;menu_items[i].item_str_pointer[j]!='\0';j++)//��߿��Ʊ���ÿ��item��ÿ����ĸ��Ѱ�ҡ�/��
		{
			if (menu_items[i].item_str_pointer[j] == '/')
			{
				count++;
			}
		}
		
		if (menu.max_deep<count)
		{
			menu.max_deep = count;
		}
	}
	
}
/*�˴��������ҵ���ǰ��ȵĲ˵����������ҽ���д��current_level_branchs[]*/
void set_level_items_amount(void)
{
	vuint8 i=0,j=0,count1=0,count2=0;
	menu.current_level_items_amount = 0;  //Ҫ����
	
	/*����ÿ���˵���*/
	for (i=0;i<menu.items_amount;i++)
	{
		count1=0;//��Ҫ��
		for (j=0;menu_items[i].item_str_pointer[j] != '\0';j++)
		{
			if (menu_items[i].item_str_pointer[j] == '/')
			{
				count1++;
			}
		}
		count2=0;
		if (count1 == menu.current_deep)
		{
			for (j=0;menu_items[i].item_str_pointer[j]==menu.current_item_str[j];j++)
			{
				if (menu_items[i].item_str_pointer[j]=='/')
				{
				  count2++;
					if(count2 == menu.current_deep)
					{
						current_level_branchs[menu.current_level_items_amount] = &menu_items[i];
						menu.current_level_items_amount += 1;
						break;
					}
				}
			}
		}
	}
	

}
void menuforward(void)
{
	vuint8 i=0;
	
	menu.deep_change_flag = 1;
	menu.current_deep++;
	menu.current_level_item_order = 1;
	
	/*��������һС�α���Ҫ�У�û�л���ô����*/
	/*û�еĻ�����level�˵������õ���/����������һ����/�����а�*/
	for (i=0;menu.current_item_str[i] != '\0';i++);
	menu.current_item_str[i] = '/';
	menu.current_item_str[i+1] = '\0';
	
	set_level_items_amount();
	
	if (menu.current_level_items_amount !=0)
	{
		menu.current_item_pointer = current_level_branchs[0];
		
		for (i++;menu.current_item_pointer->item_str_pointer[i]!='\0'&& menu.current_item_pointer->item_str_pointer[i]!='/';i++)
		{
			menu.current_item_str[i] = menu.current_item_pointer->item_str_pointer[i];
		}
		menu.current_item_str[i] = '\0';
	
	}
	else if(menu.current_level_items_amount==0)
	{
		menu.fuc_run_falg = 1;
		menuback();
	}
	
	
}

void menuback(void)
{
	vuint8 i=0,j=0,count=0;
	
	menu.deep_change_flag = 1;
	menu.current_deep--;
	set_level_items_amount();
	
	if (menu.current_deep < 1)
	{
		menu.current_level_items_amount = 1;
		menu.current_deep = 1;
	}
	/*��Ȼ��ǰ���Ǿ�Ҫȥ�����һ����ʶ�Եķ��š�/��*/
	for (i=0;menu.current_item_str[i] != '\0';i++)
	{
		if (menu.current_item_str[i] == '/')
		{
			count++;
		}
		if (count == menu.current_deep + 1)
		{
			menu.current_item_str[i] = '\0';  //����˴�û��break����ôȥ������һ�����Ჹ��\0��
		}
	}
	
	for (i=0;i<menu.current_level_items_amount;i++)
	{
		for (j=0;menu.current_item_str[j] != '\0';j++)
		{
			if (current_level_branchs[i]->item_str_pointer[j] != menu.current_item_str[j])
			{
				break;
			}
		}
		if (menu.current_item_str[j] == '\0')
		{
			menu.current_level_item_order = i+1;
			menu.current_item_pointer = current_level_branchs[i];
			break;
		}
	}
}
void menudown(void)
{
	vuint8 i=0;
	
	menu.current_level_item_order++;
	
	if (menu.current_level_item_order > menu.current_level_items_amount)
	{
		//menu.current_level_item_order = menu.current_level_items_amount;
		menu.current_level_item_order = 1; //�˵���ѭ���������һ��ֱ��������һ��
	}
	
	for (i=0;current_level_branchs[menu.current_level_item_order-1]->item_str_pointer[i]!='\0';i++)
	{
		menu.current_item_str[i] = current_level_branchs[menu.current_level_item_order-1]->item_str_pointer[i];
	}
	
	menu.current_item_str[i] = '\0';
	menu.current_item_pointer = current_level_branchs[menu.current_level_item_order-1];
}

void menuup(void)
{
	vuint8 i;
	
	menu.current_level_item_order--;
	
	if (menu.current_level_item_order<1)
	{
		//menu.current_level_item_order = 1;
		menu.current_level_item_order = menu.current_level_items_amount;//�˵���ѭ��
	}
	
	for (i=0;current_level_branchs[menu.current_level_item_order-1]->item_str_pointer[i] != '\0';i++)
	{
		menu.current_item_str[i] = current_level_branchs[menu.current_level_item_order-1]->item_str_pointer[i];
	}
	
	menu.current_item_str[i] = '\0';
	menu.current_item_pointer = current_level_branchs[menu.current_level_item_order-1];
}



void showmenu(void)
{
  vuint8 i=0,j=0,count1=0,count2=0,find_flag=0;//find_flag��ʾ�ҵ���������
  vuint8 temp_str[ITEM_LENGTH];
  char float_buff[10];
	
	if (menu.deep_change_flag == 1)//��menu_forward��1
	{
		menu.deep_change_flag = 0;
		aera_blank();
	}
	
	for (i=0; i<menu.current_level_items_amount; i++)
	{
		count1 = 0;
		count2 = 0;
		find_flag = 0;

		
		/*��Ҫ��ʾ������װ��temp_str,ѭ��current_level_items_amount��*/
		for (j=0;;j++)
		{
			if (current_level_branchs[i]->item_str_pointer[j] == '/'&& find_flag == 0)
			{
				count1++;
				
				if (count1 == menu.current_deep)
				{
					j++;
					find_flag = 1;
				}
			}
			if (find_flag == 1)
			{
				if ((current_level_branchs[i]->item_str_pointer[j] == '\0')|| (current_level_branchs[i]->item_str_pointer[j] == '/'))
				{
					find_flag = 0;
					temp_str[count2] = '\0';
					/*���������������˵���Ŀ�Ƿ���˲�������֮�����Ӧ����ʾ*/
					if (current_level_branchs[i]->ifhasparam == true)
					{
						sprintf(float_buff,"%.4f",*(current_level_branchs[i]->paramter));
						strcat((char *)(temp_str),float_buff);
					}
					break;
				}
				temp_str[count2] = current_level_branchs[i]->item_str_pointer[j];//count2��jӦ����һ����С
				count2++;
			}

		}
		
		/*��ʽ��ʾ��ѭ��current_level_items_amount��*/
		
		/*�˴���if���ڸ��˵������Ч������*/
		if (i+1 == menu.current_level_item_order)
		{
			gui_printf_chosestring(0,LINE(i+1),(char*)temp_str);
			/*version 1*/
//	        UG_SetForecolor(SELECT_COLOR);
//			UG_PutString(0,LINE(i+1),(char*)temp_str);
//			UG_SetForecolor(NORMAL_COLOR);

			/*version 2*/
//			UG_TextboxSetForeColor(&window_1,menu.current_level_item_order-1,C_RED);
//			box_print_s(&window_1,menu.current_level_item_order-1,(char*)temp_str);
//			UG_TextboxShow(&window_1,menu.current_level_item_order-1);
			
		}
		else
		{
			gui_print_s(0,LINE(i+1),(char*)temp_str);
			//UG_PutString(0,LINE(i+1),(char*)temp_str);
			
			/*version 2*/
//			UG_TextboxSetForeColor(&window_1,menu.current_level_item_order-1,C_PURPLE);
//			box_print_s(&window_1,menu.current_level_item_order-1,(char*)temp_str);
//			UG_TextboxShow(&window_1,menu.current_level_item_order-1);
		}
	}
	
	
	UpdateScreen();
	
	/*debug infor*/
//	my_printf(0,LINE(5),"D%d",menu.current_deep);
//	my_printf(20,LINE(5),"M%d",menu.current_level_items_amount);
//  my_printf(0,LINE(20),"TAG:%d",menu.current_level_item_order);
//	my_printf(60,LINE(5),"F%d",menu.deep_change_flag);
}

void exefunckeck(void)
{
	if (menu.fuc_run_falg == 1)
	{
		menu.fuc_run_falg = 0;
		aera_blank();
		if(menu.current_item_pointer->fun!=empty)
		{
			key_infor.key_state = 1;//��λ���ܱ���
			menu.current_item_pointer->fun();
			key_infor.key_state = 0;
		}
		
	}
	
}

void menuinit(void)
{
	aera_blank();
	menu_param_init();
	set_items_amount();
	set_max_deep();
	set_level_items_amount();
	menuforward();
	showmenu();
}
void chosemenu(void)
{
	switch(key_infor.key_value)
	{
		case LEFT:   menuback();     break;
		case RIGHT:  menuforward();  break;
		case UP:     menuup();       break;
		case DOWN:   menudown();     break;
		default: break;
	}
}

void change_level(void)
{
	if (key_infor.key_state == 1)
	{
		key_infor.key_state = 0;
		
		//SCAN_ENABLE = 0;
		chosemenu();
		exefunckeck();
		showmenu();
		//SCAN_ENABLE = 1;
	}
}

































