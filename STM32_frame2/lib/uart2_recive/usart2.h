/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*             ʵ�ִ���2���ܵ�ͷ�ļ�               */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __USART2_H
#define __USART2_H

#include "stdio.h"      //������Ҫ��ͷ�ļ�
#include "stdarg.h"		//������Ҫ��ͷ�ļ� 
#include "string.h"     //������Ҫ��ͷ�ļ�
#include "sys.h"


#define USART2_TXBUFF_SIZE   1024   //���崮��2 ���ͻ�������С 1024�ֽ�

#define USART2_RX_ENABLE     1      //�Ƿ������չ���  1������  0���ر�

#if  USART2_RX_ENABLE                          //���ʹ�ܽ��չ���
#define USART2_RXBUFF_SIZE   1024              //���崮��2 ���ջ�������С 1024�ֽ�

extern char Usart2_RxCompleted ;               //�ⲿ�����������ļ����Ե��øñ���
extern unsigned int Usart2_RxCounter;          //�ⲿ�����������ļ����Ե��øñ���
extern char Usart2_RxBuff[USART2_RXBUFF_SIZE]; //�ⲿ�����������ļ����Ե��øñ���
#endif


extern char rxbuf[50];
extern u8 usart2IdleItFlag;
extern u8 bufRemainByte; 

void usart2_DMA_enable(DMA_Channel_TypeDef* DMAy_Channelx,u8 bufSize);
void usart2_DMA_config(DMA_Channel_TypeDef* DMAy_Channelx,u32 memAddr,u32 periphAddr,u8 bufSize);
void usart2_config(u32 baud);

void Usart2_Init(unsigned int);       
void u2_printf(char*,...) ;          
void u2_TxData(unsigned char *data);

#endif


