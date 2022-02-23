/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*             实现串口2功能的头文件               */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __USART2_H
#define __USART2_H

#include "stdio.h"      //包含需要的头文件
#include "stdarg.h"		//包含需要的头文件 
#include "string.h"     //包含需要的头文件
#include "sys.h"


#define USART2_TXBUFF_SIZE   1024   //定义串口2 发送缓冲区大小 1024字节

#define USART2_RX_ENABLE     1      //是否开启接收功能  1：开启  0：关闭

#if  USART2_RX_ENABLE                          //如果使能接收功能
#define USART2_RXBUFF_SIZE   1024              //定义串口2 接收缓冲区大小 1024字节

extern char Usart2_RxCompleted ;               //外部声明，其他文件可以调用该变量
extern unsigned int Usart2_RxCounter;          //外部声明，其他文件可以调用该变量
extern char Usart2_RxBuff[USART2_RXBUFF_SIZE]; //外部声明，其他文件可以调用该变量
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


