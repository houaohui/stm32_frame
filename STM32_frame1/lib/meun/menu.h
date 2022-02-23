#ifndef _MENU_H
#define _MENU_H

#include "stdbool.h"
#include "common.h"
#include <stdio.h>


#define ITEM_LENGTH   40
#define LEVEL_BRANCH_MAX 8

typedef struct{
	const char* item_str_pointer;
	void        (*fun)(void);
	float       *paramter;//指向该条菜单条目所绑定的参数
	bool        ifhasparam;//指示该条菜单条目是否绑定了参数
} menu_item_t;

typedef struct{
	vuint8 max_deep;       //指示当前情况有
	vuint8 current_deep;   //指示当前情况的深度（层级）
	vuint8 items_amount;   //菜单条目总量
	vuint8 current_level_items_amount;  //当前深度有多少个分支（层）
	vuint8 current_level_item_order;    //光标所指的条目
	  
	vuint8 deep_change_flag;
	vuint8 fuc_run_falg;
	
	char               current_item_str[ITEM_LENGTH];
	menu_item_t*       current_item_pointer;
} menu_t;

/*该结构体的每一个值都是坐标中的一个元素，例如:（2，3）代表第一层的第二个的第三个item*/
typedef struct item_spot{
	int level_num[2];  //两个值，第一个值是第一层的值，0代表motor，1代表servor，未来会更多
	                   //第二个值是第二层的第n个item，0，1，2分别代表第二层的某一个item坐标
    	
    /*本次只接受2层，所以只设置了两个*/
	/*未来可能会有更多，再说*/
	//level_3_num....
}item_spot_t;

extern menu_t menu;
extern bool paraming;
extern char menu_jsoner[200];

/*debug*/
void menu_param_init(void);
void set_items_amount(void);
void set_max_deep(void);
void set_level_items_amount(void);
void showmenu(void);
/**/

void chosemenu(void);
void menuinit(void);
void aera_blank(void);
void change_level(void);

#endif /*_MENU_H*/
