/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*             ʵ�ִ���2���ܵ�Դ�ļ�               */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "usart2.h"     //������Ҫ��ͷ�ļ�
#include "wait_data_clock.h"     //������Ҫ��ͷ�ļ�

char Usart2_RxCompleted = 0;            //����һ������ 0����ʾ����δ��� 1����ʾ������� 
unsigned int Usart2_RxCounter = 0;      //����һ����������¼����2�ܹ������˶����ֽڵ�����
char Usart2_RxBuff[USART2_RXBUFF_SIZE]; //����һ�����飬���ڱ��洮��2���յ�������   	


/*-------------------------------------------------*/
/*����������ʼ������2���͹���                      */
/*��  ����bound��������                            */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void Usart2_Init(unsigned int bound)
{  	 	
    GPIO_InitTypeDef GPIO_InitStructure;     //����һ������GPIO���ܵı���
	USART_InitTypeDef USART_InitStructure;   //����һ�����ô��ڹ��ܵı���
#if USART2_RX_ENABLE                         //���ʹ�ܽ��չ���
	NVIC_InitTypeDef NVIC_InitStructure;     //���ʹ�ܽ��չ��ܣ�����һ�������жϵı���
#endif

#if USART2_RX_ENABLE                                 //���ʹ�ܽ��չ���
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�����ж��������飺��2�� �������ȼ���0 1 2 3 �����ȼ���0 1 2 3
#endif	
      
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);  //ʹ�ܴ���2ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //ʹ��GPIOAʱ��
	USART_DeInit(USART2);                                  //����2�Ĵ�����������ΪĬ��ֵ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;              //׼������PA2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO����50M
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	       //����������������ڴ���2�ķ���
    GPIO_Init(GPIOA, &GPIO_InitStructure);                 //����PA2
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;              //׼������PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //�������룬���ڴ���2�Ľ���
    GPIO_Init(GPIOA, &GPIO_InitStructure);                 //����PA3
	
	USART_InitStructure.USART_BaudRate = bound;                                    //����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //8������λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;                            //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
#if USART2_RX_ENABLE               												   //���ʹ�ܽ���ģʽ
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	               //�շ�ģʽ
#else                                                                              //�����ʹ�ܽ���ģʽ
	USART_InitStructure.USART_Mode = USART_Mode_Tx ;	                           //ֻ��ģʽ
#endif        
    USART_Init(USART2, &USART_InitStructure);                                      //���ô���2	

#if USART2_RX_ENABLE  	         					        //���ʹ�ܽ���ģʽ
	USART_ClearFlag(USART2, USART_FLAG_RXNE);	            //������ձ�־λ
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);          //���������ж�
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;       //���ô���2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                        //���ô���2�ж�
#endif  

	USART_Cmd(USART2, ENABLE);                              //ʹ�ܴ���2
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

	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//ʹ�ܴ���1�Ŀ����жϣ�ע�����ﲢû��ʹ�ܽ����жϣ�

}
USART_Cmd(USART2, ENABLE);
}


void usart2_DMA_config(DMA_Channel_TypeDef* DMAy_Channelx,u32 memAddr,u32 periphAddr,u8 bufSize)
{
DMA_InitTypeDef  DMA_InitStructure;

RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//DMA1???? 
/* ��ʼ��DMA */
DMA_InitStructure.DMA_PeripheralBaseAddr = periphAddr;//	DMA�����ַ
DMA_InitStructure.DMA_MemoryBaseAddr = memAddr;//			DMA�ڴ��ַ
DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//		�����䷽������赽�ڴ�
DMA_InitStructure.DMA_BufferSize = bufSize;					//�������ݴ�С
DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//����һ�κ������ַ������
DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//����һ�κ��ڴ��ַ����
DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�����ֽڴ���
DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�����ֽڴ���
DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//����ģʽ
DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//���ȼ��еȣ�һ��DMAͨ�������Ļ����Բ�����
DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //�ر��ڴ浽�ڴ�
DMA_Init(DMAy_Channelx, &DMA_InitStructure);//��ʼ��


}

void usart2_DMA_enable(DMA_Channel_TypeDef* DMAy_Channelx,u8 bufSize)
{
USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);//DMA��������ʹ��
DMA_Cmd(DMAy_Channelx, DISABLE);//�ȹرղ���д���͵�����
DMA_SetCurrDataCounter(DMAy_Channelx, bufSize);//д�봫�������������ֵΪ0DMA������
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
		clear = USART_ReceiveData(USART2);//ͨ���ȶ�״̬�Ĵ����ٶ����ݼĴ�����IDLE���
		usart2IdleItFlag = 1;//������ɱ�־λ��1
		bufRemainByte = 50 - DMA_GetCurrDataCounter(DMA1_Channel6);//���㱾�δ��͵��ֽ�����������50Ϊ����
	}
}
















/*-------------------------------------------------*/
/*������������2 printf����                         */
/*��  ����char* fmt,...  ��ʽ������ַ����Ͳ���    */
/*����ֵ����                                       */
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
/*������������2���ͻ������е�����                  */
/*��  ����data������                               */
/*����ֵ����                                       */
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
/*������������2�����жϺ���                        */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/

extern char is_wifi_data_start;
extern unsigned char wifi_time_cnt;

//void USART2_IRQHandler(void)   
//{
//	
//	//���USART_IT_RXNE��־��λ����ʾ�����ݵ��ˣ�����if��֧
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//	{  
//		if(TCP_flag==0)
//		{                                    //���UDP_flag����0����ǰ��ʼ��UDP��δ�ɹ�������ָ������״̬
//			if(USART2->DR)
//			{                                 //����ָ������״̬ʱ������ֵ�ű��浽������	
//				Usart2_RxBuff[Usart2_RxCounter]=USART2->DR; //���浽������	
//				Usart2_RxCounter ++;                        //ÿ����1���ֽڵ����ݣ�Usart2_RxCounter��1����ʾ���յ���������+1 
//			}		
//		}
//		else
//		{		                                        //��֮UDP_flag����1����ʼ��UDP�ɹ�
//			Usart2_RxBuff[Usart2_RxCounter] = USART2->DR;   //�ѽ��յ������ݱ��浽Usart2_RxBuff��				
//			if(Usart2_RxCounter == 0)
//			{    					//���Usart2_RxCounter����0����ʾ�ǽ��յĵ�1�����ݣ�����if��֧				
//				is_wifi_data_start=1; 
//			}
//			else
//			{                        					//else��֧����ʾ��Usart2_RxCounter������0�����ǽ��յĵ�һ������
//				wifi_time_cnt=0;;  
//			}
//			Usart2_RxCounter ++;         				    //ÿ����1���ֽڵ����ݣ�Usart2_RxCounter��1����ʾ���յ���������+1 
//		}
//	}
//}



