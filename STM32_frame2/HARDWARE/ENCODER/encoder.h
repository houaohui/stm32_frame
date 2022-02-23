#ifndef __ENCODER_H
#define __ENCODER_H
#include <sys.h>	 
/**************************************************************************
��    �ߣ��������
�Ա���ַ��https://shop119207236.taobao.com
**************************************************************************/
#define ENCODER_TIM_PERIOD (u16)(65535)   //103�Ķ�ʱ����16λ 2��16�η������65536

extern int Encoder_L;
extern int Encoder_R;

void Encoder_Init_TIM2(void);
void Encoder_Init_TIM4(void);
int Read_Encoder(u8 TIMX);
void TIM4_IRQHandler(void);
void TIM2_IRQHandler(void);
#endif
