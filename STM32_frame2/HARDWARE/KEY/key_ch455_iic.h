#ifndef _KEY_CH455_IIC_H_
#define _KEY_CH455_IIC_H_

#include "stdbool.h"
#include "sys.h"
#include "gui.h"
#include "myinclude.h"

#if USE_CH455_KEY
typedef unsigned char   UINT8;
typedef unsigned short  UINT16;


  typedef enum{
	   GPI=0,
	   GPO=1
   }GPIO_Dir;
 

/*使用官方的例程实现*/
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
////////////////////////////////////////////////////////////
///////引脚定义////////////
////////////////////////////////////////////////////////////
#define     CH455_IIC_DELAY    ch455_iic_delay(30)

#define     CH455_SCL_H        GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define     CH455_SCL_L        GPIO_ResetBits(GPIOB,GPIO_Pin_12)

#define     CH455_SDA_H        GPIO_SetBits(GPIOB,GPIO_Pin_13)
#define     CH455_SDA_L        GPIO_ResetBits(GPIOB,GPIO_Pin_13)

#define     CH455_SDA_IN       PBin(13)

#define     CH455_SDA_D_OUT    gpio_dir(GPIOB,13,GPO);
#define     CH455_SDA_D_IN     gpio_dir(GPIOB,13,GPI);


//#define     USE_CH455_KEY       1

//#ifdef USE_CH455_KEY
//    #define     DISABLE_KEY_INTERRUPT   {}
//    #define     ENABLE_KEY_INTERRUPT    {}
//    #define     CLEAR_KEY_INTER_FLAG    {}
//#else
//    #define     DISABLE_KEY_INTERRUPT   {}
//    #define     ENABLE_KEY_INTERRUPT    {}
//    #define     CLEAR_KEY_INTER_FLAG    {}
//#endif

#define CH455_BIT_ENABLE    0x01
#define CH455_BIT_SLEEP     0x04
#define CH455_BIT_7SEG      0x08
#define CH455_BIT_INTENS1   0x10
#define CH455_BIT_INTENS2   0x20
#define CH455_BIT_INTENS3   0x30
#define CH455_BIT_INTENS4   0x40
#define CH455_BIT_INTENS5   0x50
#define CH455_BIT_INTENS6   0x60
#define CH455_BIT_INTENS7   0x70
#define CH455_BIT_INTENS8   0x00

#define CH455_SYSOFF    0x0400
#define CH455_SYSON     ( CH455_SYSOFF | CH455_BIT_ENABLE )
#define CH450_SYSON2    0x0403
#define CH455_SLEEPOFF  CH455_SYSOFF
#define CH455_SLEEPON   ( CH455_SYSOFF | CH455_BIT_SLEEP )
#define CH455_7SEG_ON   ( CH455_SYSON | CH455_BIT_7SEG )
#define CH455_8SEG_ON   ( CH455_SYSON | 0x00 )
#define CH455_SYSON_4   ( CH455_SYSON | CH455_BIT_INTENS4 )
#define CH455_SYSON_8   ( CH455_SYSON | CH455_BIT_INTENS8 )



#define CH455_DIG0      0x1400
#define CH455_DIG1      0x1500
#define CH455_DIG2      0x1600
#define CH455_DIG3      0x1700

#define CH455_GET_KEY   0x0700

#define     CH455_I2C_ADDR      0x40
#define     CH455_I2C_MASK      0x3E



void gpio_dir(GPIO_TypeDef* GPIOx, uint16_t GPIO_num , unsigned char Dir);
void ch455_iic_delay(int t);


vuint8 ch455_read_key_value(void);
void CH455_Key_Init(void);

unsigned char key_info_set(void);

#endif
#endif /*_KEY_IIC_H_*/
