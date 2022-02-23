/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*             实现串口2功能的源文件               */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //包含需要的头文件
#include "usart2.h"     //包含需要的头文件
#include "wait_data_clock.h"     //包含需要的头文件

char Usart2_RxCompleted = 0;            //定义一个变量 0：表示接收未完成 1：表示接收完成 
unsigned int Usart2_RxCounter = 0;      //定义一个变量，记录串口2总共接收了多少字节的数据
char Usart2_RxBuff[USART2_RXBUFF_SIZE]; //定义一个数组，用于保存串口2接收到的数据   	


/*-------------------------------------------------*/
/*函数名：初始化串口2发送功能                      */
/*参  数：bound：波特率                            */
/*返回值：无                                       */
/*-------------------------------------------------*/
void Usart2_Init(unsigned int bound)
{  	 	
    GPIO_InitTypeDef GPIO_InitStructure;     //定义一个设置GPIO功能的变量
	USART_InitTypeDef USART_InitStructure;   //定义一个设置串口功能的变量
#if USART2_RX_ENABLE                         //如果使能接收功能
	NVIC_InitTypeDef NVIC_InitStructure;     //如果使能接收功能，定义一个设置中断的变量
#endif

#if USART2_RX_ENABLE                                 //如果使能接收功能
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3
#endif	
      
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);  //使能串口2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //使能GPIOA时钟
	USART_DeInit(USART2);                                  //串口2寄存器重新设置为默认值
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;              //准备设置PA2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO速率50M
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	       //复用推挽输出，用于串口2的发送
    GPIO_Init(GPIOA, &GPIO_InitStructure);                 //设置PA2
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;              //准备设置PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入，用于串口2的接收
    GPIO_Init(GPIOA, &GPIO_InitStructure);                 //设置PA3
	
	USART_InitStructure.USART_BaudRate = bound;                                    //波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //8个数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;                            //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
#if USART2_RX_ENABLE               												   //如果使能接收模式
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	               //收发模式
#else                                                                              //如果不使能接收模式
	USART_InitStructure.USART_Mode = USART_Mode_Tx ;	                           //只发模式
#endif        
    USART_Init(USART2, &USART_InitStructure);                                      //设置串口2	

#if USART2_RX_ENABLE  	         					        //如果使能接收模式
	USART_ClearFlag(USART2, USART_FLAG_RXNE);	            //清除接收标志位
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);          //开启接收中断
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;       //设置串口2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//中断通道使能
	NVIC_Init(&NVIC_InitStructure);	                        //设置串口2中断
#endif  

	USART_Cmd(USART2, ENABLE);                              //使能串口2
}




void usart2_config(u32 baud)
{
GPIO_InitTypeDef io;
USART_InitTypeDef temp;
//RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);
RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);

io.GPIO_Pin = GPIO_Pin_2;
io.GPIO_Mode = GPIO_Mode_Out_PP;//txd
io.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &io);
	
PAout(2)=1;

io.GPIO_Pin = GPIO_Pin_3;
io.GPIO_Mode = GPIO_Mode_IN_FLOATING;//rxd
io.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &io);


temp.USART_BaudRate = baud;
temp.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
temp.USART_Mode = USART_Mode_Rx;
temp.USART_Parity = USART_Parity_No;
temp.USART_StopBits = USART_StopBits_1;
temp.USART_WordLength = USART_WordLength_8b;
USART_Init(USART2, &temp);

{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//使能串口1的空闲中断，注意这里并没有使能接收中断！

}
USART_Cmd(USART2, ENABLE);
}


void usart2_DMA_config(DMA_Channel_TypeDef* DMAy_Channelx,u32 memAddr,u32 periphAddr,u8 bufSize)
{
DMA_InitTypeDef  DMA_InitStructure;

RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//DMA1???? 
/* 初始化DMA */
DMA_InitStructure.DMA_PeripheralBaseAddr = periphAddr;//	DMA外设地址
DMA_InitStructure.DMA_MemoryBaseAddr = memAddr;//			DMA内存地址
DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//		·传输方向从外设到内存
DMA_InitStructure.DMA_BufferSize = bufSize;					//传输数据大小
DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//传输一次后外设地址不增加
DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//传输一次后内存地址增加
DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//按照字节传送
DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//按照字节传送
DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//正常模式
DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//优先级中等，一个DMA通道工作的话可以不设置
DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //关闭内存到内存
DMA_Init(DMAy_Channelx, &DMA_InitStructure);//初始化


}

void usart2_DMA_enable(DMA_Channel_TypeDef* DMAy_Channelx,u8 bufSize)
{
USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);//DMA接收请求使能
DMA_Cmd(DMAy_Channelx, DISABLE);//先关闭才能写传送的数据
DMA_SetCurrDataCounter(DMAy_Channelx, bufSize);//写入传送数量，如果此值为0DMA开不了
DMA_Cmd(DMAy_Channelx, ENABLE);
}


u8 usart2IdleItFlag;
u8 bufRemainByte; 


char rxbuf[50];

void USART2_IRQHandler(void)
{
    u16 clear;
	if(USART_GetFlagStatus(USART2, USART_FLAG_IDLE))
	{
		clear = USART_ReceiveData(USART2);//通过先读状态寄存器再读数据寄存器将IDLE清除
		usart2IdleItFlag = 1;//接收完成标志位置1
		bufRemainByte = 50 - DMA_GetCurrDataCounter(DMA1_Channel6);//计算本次传送的字节数。总数以50为例子
	}
}
















/*-------------------------------------------------*/
/*函数名：串口2 printf函数                         */
/*参  数：char* fmt,...  格式化输出字符串和参数    */
/*返回值：无                                       */
/*-------------------------------------------------*/

__align(8) char USART2_TxBuff[USART2_TXBUFF_SIZE];  

void u2_printf(char* fmt,...) 
{  
	unsigned int i,length;
	
	va_list ap;
	va_start(ap,fmt);
	vsprintf(USART2_TxBuff,fmt,ap);
	va_end(ap);	
	
	length=strlen((const char*)USART2_TxBuff);		
	while((USART2->SR&0X40)==0);
	for(i = 0;i < length;i ++)
	{			
		USART2->DR = USART2_TxBuff[i];
		while((USART2->SR&0X40)==0);	
	}	
}

/*-------------------------------------------------*/
/*函数名：串口2发送缓冲区中的数据                  */
/*参  数：data：数据                               */
/*返回值：无                                       */
/*-------------------------------------------------*/
void u2_TxData(unsigned char *data)
{
	int	i;	
	while((USART2->SR&0X40)==0);
	for(i = 1;i <= data[0];i ++){			
		USART2->DR = data[i];
		while((USART2->SR&0X40)==0);	
	}
}



/*-------------------------------------------------*/
/*函数名：串口2接收中断函数                        */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/

extern char is_wifi_data_start;
extern unsigned char wifi_time_cnt;

//void USART2_IRQHandler(void)   
//{
//	
//	//如果USART_IT_RXNE标志置位，表示有数据到了，进入if分支
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//	{  
//		if(TCP_flag==0)
//		{                                    //如果UDP_flag等于0，当前初始化UDP还未成功，处于指令配置状态
//			if(USART2->DR)
//			{                                 //处于指令配置状态时，非零值才保存到缓冲区	
//				Usart2_RxBuff[Usart2_RxCounter]=USART2->DR; //保存到缓冲区	
//				Usart2_RxCounter ++;                        //每接收1个字节的数据，Usart2_RxCounter加1，表示接收的数据总量+1 
//			}		
//		}
//		else
//		{		                                        //反之UDP_flag等于1，初始化UDP成功
//			Usart2_RxBuff[Usart2_RxCounter] = USART2->DR;   //把接收到的数据保存到Usart2_RxBuff中				
//			if(Usart2_RxCounter == 0)
//			{    					//如果Usart2_RxCounter等于0，表示是接收的第1个数据，进入if分支				
//				is_wifi_data_start=1; 
//			}
//			else
//			{                        					//else分支，表示果Usart2_RxCounter不等于0，不是接收的第一个数据
//				wifi_time_cnt=0;;  
//			}
//			Usart2_RxCounter ++;         				    //每接收1个字节的数据，Usart2_RxCounter加1，表示接收的数据总量+1 
//		}
//	}
//}



