#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK mini�SSTM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��

void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PA,PD�˿�ʱ��
//	
// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED0-->PA.8 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
// GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.8
// GPIO_SetBits(GPIOA,GPIO_Pin_8);						 //PA.8 �����

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	    		 //LED1-->PD.2 �˿�����, �������
 GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_ResetBits(GPIOC,GPIO_Pin_13); 						 //PD.2 ����� 
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;	    		 //LED1-->PD.2 �˿�����, �������
 GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_SetBits(GPIOC,GPIO_Pin_14); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	    		 //LED1-->PD.2 �˿�����, �������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_ResetBits(GPIOC,GPIO_Pin_15); 
 
 
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	    		 //LED1-->PD.2 �˿�����, �������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz

 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	    		 //LED1-->PD.2 �˿�����, �������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	    		 //LED1-->PD.2 �˿�����, �������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_ResetBits(GPIOA,GPIO_Pin_15); 
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	    		 //LED1-->PD.2 �˿�����, �������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_ResetBits(GPIOA,GPIO_Pin_12); 
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	    		 //LED1-->PD.2 �˿�����, �������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //���ó���������
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_SetBits(GPIOB,GPIO_Pin_8); 
 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PD.2 �˿�����, �������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //���óɸ�������
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_SetBits(GPIOB,GPIO_Pin_5); 
 
}						 //PD.2 ����� 
 