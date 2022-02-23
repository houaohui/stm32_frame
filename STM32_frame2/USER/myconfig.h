#ifndef _MYCONFIG_H_
#define _MYCONFIG_H_

//////////////////////////////////////////////////////////////////////////////////	 
//¶¨Ê±Æ÷    TIM3   1ms
//PWM        TIM1   PA8(ch1) PA9(ch2) PA10(ch3)  PA11
//±àÂëÆ÷     TIM2   PA0 PA1    TIM4 PB6 PB7
//ADC(12bit)     ADC1 PA4
//UART           PA9(TX)   PA10(RX)
//UART2          PA2(TX)       PA3
//LED       PC13    PC14
//CH455     PB12(SCL)   PB13(SDA)    PB14(EXIT)
//OLED      PA5(D0)     PA7(D1)      PB0(RST)      PB1(DC)     PA4(CS)
//MPU6050   PB10(SCL)    PB11(SDA)
//ÂÌµÆPC14  ºìµÆPA2   »ÆµÆPB8 ¿ª¹ØPC15
//PB5  ¼ì²âµçÆ½±ä»»
//PB3 18   PB4   19   PA12  16   PA15  17
////////////////////////////////////////////////////////////////////////////////// 	  

#define USE_CH455_KEY  1
#if USE_CH455_KEY

#define CH455_KEY_SCAN 0
#define CH455_KEY_INTER  1

#endif




#endif

