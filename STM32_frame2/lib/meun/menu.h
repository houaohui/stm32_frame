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
	float       *paramter;//ָ������˵���Ŀ���󶨵Ĳ���
	bool        ifhasparam;//ָʾ�����˵���Ŀ�Ƿ���˲���
} menu_item_t;

typedef struct{
	vuint8 max_deep;       //ָʾ��ǰ�����
	vuint8 current_deep;   //ָʾ��ǰ�������ȣ��㼶��
	vuint8 items_amount;   //�˵���Ŀ����
	vuint8 current_level_items_amount;  //��ǰ����ж��ٸ���֧���㣩
	vuint8 current_level_item_order;    //�����ָ����Ŀ
	  
	vuint8 deep_change_flag;
	vuint8 fuc_run_falg;
	
	char               current_item_str[ITEM_LENGTH];
	menu_item_t*       current_item_pointer;
} menu_t;

/*�ýṹ���ÿһ��ֵ���������е�һ��Ԫ�أ�����:��2��3�������һ��ĵڶ����ĵ�����item*/
typedef struct item_spot{
	int level_num[2];  //����ֵ����һ��ֵ�ǵ�һ���ֵ��0����motor��1����servor��δ�������
	                   //�ڶ���ֵ�ǵڶ���ĵ�n��item��0��1��2�ֱ����ڶ����ĳһ��item����
    	
    /*����ֻ����2�㣬����ֻ����������*/
	/*δ�����ܻ��и��࣬��˵*/
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
