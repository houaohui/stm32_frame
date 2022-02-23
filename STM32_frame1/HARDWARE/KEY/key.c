#include "key.h"
#include "delay.h"
#include "usart.h"


unsigned char if_use_ch455=1;

unsigned char SHIFT_ON = 0;

//按键初始化函数 
//PA0.15和PC5 设置成输入

key_infor_t key_infor;



void KEY_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//使能PORTA,PORTC时钟

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;//PA15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA15
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;//PC5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC5
 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0
	
} 




//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//返回值：
//0，没有任何按键按下
//KEY0_PRES，KEY0按下
//KEY1_PRES，KEY1按下
//WKUP_PRES，WK_UP按下 
//注意此函数有响应优先级,KEY0>KEY1>WK_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY0==0)return KEY0_PRES;
		else if(KEY1==0)return KEY1_PRES;
		else if(WK_UP==1)return WKUP_PRES; 
	}else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1; 	     
	return 0;// 无按键按下
}



/////////////////////////

//portB的高八位用于驱动矩阵按键
void Matrix_button_Init(void) 
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);//使能PORTB时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	 GPIO_Init(GPIOB, &GPIO_InitStructure);					 
	 GPIO_SetBits(GPIOB,GPIO_Pin_12);						 

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	    		
	 GPIO_Init(GPIOB, &GPIO_InitStructure);	  			
	 GPIO_SetBits(GPIOB,GPIO_Pin_13); 					
	 
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;	    	
	 GPIO_Init(GPIOB, &GPIO_InitStructure);	  			
	 GPIO_SetBits(GPIOB,GPIO_Pin_14); 						
	 
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	    		
	 GPIO_Init(GPIOB, &GPIO_InitStructure);	  			
	 GPIO_SetBits(GPIOB,GPIO_Pin_15); 
	
}


/* 是否滤波 */

#define WARE_FILTERING  0

unsigned char keysta[4][4]={
	{1, 1, 1, 1}, 
	{1, 1, 1, 1},
	{1, 1, 1, 1},
	{1, 1, 1, 1}
};

const unsigned char keycodemap[4][4] = {
	{  '1',   '2',   '3',   '0'  },
	{  '4',   '5',   '6',  'e'   },
	{  '7',   '8',   '9',  'n'   },
	{  'u',   'l',   'd',  'r'   }
};


//按键扫描函数，在TIM3中断中执行
unsigned char keyscan(void)
{
	unsigned char i,j;
	unsigned char sta=' ';
	static unsigned char keyout=0;
//	static unsigned char keybuf[4][4]={
//	{0xff,0xff,0xff,0xff},{0xff,0xff,0xff,0xff},
//	{0xff,0xff,0xff,0xff},{0xff,0xff,0xff,0xff}
//	};
	
	keyout++;
	keyout=keyout&0x03;  //整个键盘扫描需要四次，采样率较低
	
	switch(keyout)   //或运算防止影响低八位
	{
		case 0: GPIOB->ODR|=0xef00;PBout(12)=0;break;
		case 1: GPIOB->ODR|=0xdf00;PBout(13)=0;break;
		case 2: GPIOB->ODR|=0xbf00;PBout(14)=0;break;
		case 3: GPIOB->ODR|=0x7f00;PBout(15)=0;break;
		default:break;
	}
	
	
	
#if WARE_FILTERING
	
	keybuf[keyout][0]=(keybuf[keyout][0]<<1)|PBin(11);
	keybuf[keyout][1]=(keybuf[keyout][1]<<1)|PBin(10);
	keybuf[keyout][2]=(keybuf[keyout][2]<<1)|PBin(9);
	keybuf[keyout][3]=(keybuf[keyout][3]<<1)|PBin(8);

	for(i=0;i<4;i++)
	{
		if((keybuf[keyout][i]&0x0f)==0x00)
		{
			keysta[keyout][i]=0;
			
		}
		else if((keybuf[keyout][i]&0x0f)==0x0f)
		{
			keysta[keyout][i]=1;
			
		}
	}
	
	for (i=0; i<4; i++)
	{
		for (j=0; j<4; j++)
		{
			if (keysta[i][j]==0)
			{
				sta = keycodemap[i][j];
				//printf("L%d..C%d:::%d\r\n",i,j,sta);
			}
			
				
		}
	}
#else
	//直接把引脚的电平赋给按键值，减少扫描时间，同样需要四次才能扫描完整个键盘
	keysta[keyout][0]=PBin(11);
	keysta[keyout][1]=PBin(10);
	keysta[keyout][2]=PBin(9);
	keysta[keyout][3]=PBin(8);

	for (i=0; i<4; i++)
	{
		for (j=0; j<4; j++)
		{
			if (keysta[i][j]==0)
			{
				sta = keycodemap[i][j];
				//printf("L%d..C%d:::%d\r\n",i,j,sta);
			}
		}
	}

#endif
	return sta;
}




void buttons_updata(void)
{

	static millis8_t lastUpdate;

	// Update every 20ms
	millis8_t now = millis();
	if ((millis8_t)(now - lastUpdate) >= 10)
	{
		lastUpdate = now;
		if(key_infor.key_backup!=key_infor.key_state)
		{
			if (key_infor.key_state==1)    //菜单运行！！
			{
				change_level();
			}
			key_infor.key_backup=key_infor.key_state;
		}
	
	}
	
	
}









