#include "menu.h"
#include "draw_api.h"
#include "func.h"
#include "string.h"
#include "key.h"
#include "gui.h"
/*包含参数要用到的头文件*/
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
	menu.max_deep = 0;       //先置0，后边专门算max_deep
	menu.current_deep = 1;   
	menu.items_amount = 0;   //先置0，后边专门算items_amount
	menu.current_level_items_amount = 1;     //初始化的时候仅仅显示root，因而置1
	menu.current_level_item_order = 1;       //每次都是第一个，而后再变
	menu.deep_change_flag = 0;
	menu.fuc_run_falg = 0;
	menu.current_item_pointer = &menu_items[0];     //当前菜单暂且是menu_items[0]
	//
	/*注意：current_item_str是menu的属性，item_str_pointer是菜单项的属性*/
	/*此处暂且将menu[0]的字符装入current_item_str*/
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
	
	for(i=0;i<menu.items_amount;i++) //外边控制数到第几个item
	{
		count = 0;
		
		for (j=0;menu_items[i].item_str_pointer[j]!='\0';j++)//里边控制遍历每个item的每个字母来寻找‘/’
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
/*此处的任务：找到当前深度的菜单个数，并且将其写入current_level_branchs[]*/
void set_level_items_amount(void)
{
	vuint8 i=0,j=0,count1=0,count2=0;
	menu.current_level_items_amount = 0;  //要清零
	
	/*遍历每个菜单项*/
	for (i=0;i<menu.items_amount;i++)
	{
		count1=0;//必要！
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
	
	/*接下来这一小段必须要有，没有会怎么样？*/
	/*没有的话，找level菜单数量用到‘/’，不补充一个’/‘不行啊*/
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
	/*既然向前，那就要去掉最后一个标识性的符号‘/’*/
	for (i=0;menu.current_item_str[i] != '\0';i++)
	{
		if (menu.current_item_str[i] == '/')
		{
			count++;
		}
		if (count == menu.current_deep + 1)
		{
			menu.current_item_str[i] = '\0';  //如果此处没有break，那么去掉的那一级将会补‘\0’
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
		menu.current_level_item_order = 1; //菜单下循环，从最后一个直接跳到第一个
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
		menu.current_level_item_order = menu.current_level_items_amount;//菜单上循环
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
  vuint8 i=0,j=0,count1=0,count2=0,find_flag=0;//find_flag表示找到单个单词
  vuint8 temp_str[ITEM_LENGTH];
  char float_buff[10];
	
	if (menu.deep_change_flag == 1)//在menu_forward置1
	{
		menu.deep_change_flag = 0;
		aera_blank();
	}
	
	for (i=0; i<menu.current_level_items_amount; i++)
	{
		count1 = 0;
		count2 = 0;
		find_flag = 0;

		
		/*将要显示的内容装入temp_str,循环current_level_items_amount次*/
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
					/*下面五行用来检查菜单条目是否绑定了参数，绑定之后会相应的显示*/
					if (current_level_branchs[i]->ifhasparam == true)
					{
						sprintf(float_buff,"%.4f",*(current_level_branchs[i]->paramter));
						strcat((char *)(temp_str),float_buff);
					}
					break;
				}
				temp_str[count2] = current_level_branchs[i]->item_str_pointer[j];//count2和j应当是一样大小
				count2++;
			}

		}
		
		/*正式显示，循环current_level_items_amount次*/
		
		/*此处的if用于给菜单项加特效！！！*/
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
			key_infor.key_state = 1;//置位才能避免
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

































