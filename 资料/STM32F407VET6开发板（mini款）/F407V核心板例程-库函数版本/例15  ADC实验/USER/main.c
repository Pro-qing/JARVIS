#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "GUI.h"
#include "Lcd_Driver.h"
#include "adc.h"


//STM32F407������ 
//ADCģ��ת��ʵ��-�⺯���汾  

 	u16 adcx;
	float temp;
  u8 tbuf[40];	//��ʾ������

int main(void)
{ 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED 
	
  Lcd_Init();	 //1.8��Һ����--��ʼ������
	Lcd_Clear(GRAY0);//����
	
	Adc_Init();         //��ʼ��ADC
	
	Gui_DrawFont_GBK16(0,16,RED,GRAY0," STM32F4xx ����");
	Gui_DrawFont_GBK16(0,32,RED,GRAY0,"  ADC TEST");	
	Gui_DrawFont_GBK16(0,48,RED,GRAY0,"  2018/5/5");	

	
	Gui_DrawFont_GBK16(4,64,BLUE,GRAY0,"ADC1_CH5_VAL:");	      
	Gui_DrawFont_GBK16(4,80,BLUE,GRAY0,"ADC1_CH5_VOL:0.000V");	//���ڹ̶�λ����ʾС����      
	while(1)
	{
		
		adcx=Get_Adc_Average(ADC_Channel_5,20);//��ȡͨ��5��ת��ֵ��20��ȡƽ��
		
				
		sprintf((char*)tbuf,"ADC:0x%x",adcx); //��ʾADCC�������ԭʼֵ
		Gui_DrawFont_GBK16(4,64,BLUE,GRAY0,"                 ");	//����ϴε���ʾ
		Gui_DrawFont_GBK16(4,64,BLUE,GRAY0,tbuf);	//Һ������ʾ�����
		
		temp=(float)adcx*(3.3/4096);          //��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111

		sprintf((char*)tbuf,"ADC: %5.3f",temp); //��ʾ������
		Gui_DrawFont_GBK16(4,80,BLUE,GRAY0,"                 ");	//����ϴε���ʾ
		Gui_DrawFont_GBK16(4,80,BLUE,GRAY0,tbuf);	//Һ������ʾ�����
		
		LED2=!LED2;
		delay_ms(250);	
		
	}
}



