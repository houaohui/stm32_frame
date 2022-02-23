#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
#include "common.h"
#include "key_ch455_iic.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK miniSTM32开发板
//按键驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 


//#define KEY0 PCin(5)   	
//#define KEY1 PAin(15)	 
//#define WK_UP  PAin(0)	 
   
 
 typedef struct{ 
vuint8 key_state ;
vuint8 key_value ;
	 uint8_t key_backup ;
}key_infor_t;
 
#define ZERO   0
#define ONE    1
#define TWO    2
#define THREE  3
#define FOUR   4
#define FIVE   5
#define SIX    6
#define SEVEN  7
#define EIGHT  8
#define NINE   9
#define UP     10
#define DOWN   11
#define LEFT   12
#define RIGHT  13
#define OK     14
#define BACK   15
#define MINUS  16
#define POINT  17

extern unsigned char SHIFT_ON;
extern key_infor_t key_infor;

#define KEY0  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)//读取按键0
#define KEY1  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)//读取按键1
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键2 
 

#define KEY0_PRES	1		//KEY0  
#define KEY1_PRES	2		//KEY1 
#define WKUP_PRES	3		//WK_UP  

extern unsigned char if_use_ch455;

void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8 mode);  	//按键扫描函数	

//矩阵按键
void Matrix_button_Init(void);
unsigned char keyscan(void);

void gpio_dir(GPIO_TypeDef* GPIOx, uint16_t GPIO_num , unsigned char Dir);

void buttons_updata(void);


#endif
