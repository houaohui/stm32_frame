#include "adc.h"

_ADC_VALUE bat = {0 ,0 };


uint16_t adc_value[1];

/* �˿����ó�ʼ�� */
void adc_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_initStructure;    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_initStructure.GPIO_Pin = GPIO_Pin_4;	    
	GPIO_initStructure.GPIO_Mode = GPIO_Mode_AIN;								    
	GPIO_Init(GPIOA,&GPIO_initStructure);	
}

/* adc���� */ 
void adc_config(void)
{
    ADC_InitTypeDef ADC_initStructure;
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	ADC_initStructure.ADC_ContinuousConvMode = ENABLE;					        //����ת��	
	ADC_initStructure.ADC_DataAlign = ADC_DataAlign_Right;		                //�����Ҷ���				
	ADC_initStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	        //������� 
	ADC_initStructure.ADC_Mode = ADC_Mode_Independent;							
	ADC_initStructure.ADC_NbrOfChannel = 1;										
	ADC_initStructure.ADC_ScanConvMode = DISABLE;		                        //ɨ��ģʽ						
	ADC_Init(ADC1,&ADC_initStructure);

	ADC_Cmd(ADC1,ENABLE);
    
	ADC_DMACmd(ADC1,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);                                           //ADCʱ�ӷ�Ƶ											
                                                                                //ͨ�����ã�����ʱ������
	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,1,ADC_SampleTime_239Cycles5);
	
	ADC_ResetCalibration(ADC1);	                                                //��λУ׼				
	while(ADC_GetCalibrationStatus(ADC1));		                                //�ȴ�
	ADC_StartCalibration(ADC1);					                                //����У׼
	while(ADC_GetCalibrationStatus(ADC1));		                                //�ȴ�У׼���

	ADC_SoftwareStartConvCmd(ADC1,ENABLE);	                                    //����ת��                                     
}

void dma_config(void)
{
	DMA_InitTypeDef DMA_initStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_initStructure.DMA_BufferSize = 1;										
	DMA_initStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	                        //���䷽������->�ڴ�						
	DMA_initStructure.DMA_M2M = DMA_M2M_Disable;								
	DMA_initStructure.DMA_MemoryBaseAddr = (u32)adc_value;				        
	DMA_initStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         //DMA������ڴ����ݴ�С������Ϊ��λ			
	DMA_initStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//�ڴ��ַ����
	DMA_initStructure.DMA_Mode = DMA_Mode_Circular;								
	DMA_initStructure.DMA_PeripheralBaseAddr = ((u32)&ADC1->DR);	            //�����ַ			
	DMA_initStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//DMA������������ݴ�С������Ϊ��λ
	DMA_initStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//�����ַ����
	DMA_initStructure.DMA_Priority = DMA_Priority_Medium;						
	DMA_Init(DMA1_Channel1,&DMA_initStructure);
    
	DMA_Cmd(DMA1_Channel1,ENABLE);	
}


void adc_init(void)
{
    adc_gpio_init();
    adc_config();
    dma_config();
}


//��ص�ѹ���
void voltage_detection(void)
{
    bat.adc = adc_value[0];
    bat.voltage = (float)(bat.adc)/4096.0f*3.3f*2.0f;   
    
    if(bat.voltage<=3.5)
    {
        bat.danger_flag = 1;
    }
    else
    {
        bat.danger_flag = 0;
    }
}    








