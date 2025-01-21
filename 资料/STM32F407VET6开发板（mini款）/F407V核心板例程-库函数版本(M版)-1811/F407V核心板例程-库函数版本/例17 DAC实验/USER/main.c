#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

#include "GUI.h"
#include "Lcd_Driver.h"

#include "adc.h"
#include "dac.h"
#include "key.h"


//STM32F407������ 
//DACʵ��-�⺯���汾


	u8 key;	
  u8 tbuf[40];	//��ʾ������
	
int main(void)
{ 
  u16 adcx;
	float temp;
 	u8 t=0;	 
	u16 dacval=0;
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);      //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	
	LED_Init();					//��ʼ��LED 
 
	Lcd_Init();	 //1.8��Һ����--��ʼ������
	Lcd_Clear(GRAY0);//����
	
	Adc_Init(); 				//adc��ʼ��
	KEY_Init(); 				//������ʼ��
	Dac1_Init();		 		//DACͨ��1��ʼ��	
	
	
	Gui_DrawFont_GBK16(0,16,RED,GRAY0," STM32F4xx ����");
	Gui_DrawFont_GBK16(0,32,RED,GRAY0,"  DAC TEST");	
	Gui_DrawFont_GBK16(0,48,RED,GRAY0,"  2018/5/5");
	Gui_DrawFont_GBK16(0,64,RED,GRAY0,"  K1::+  ");	 
	
	    	 
	Gui_DrawFont_GBK16(0,80, BLUE,GRAY0," DAC VAL:");	      
	Gui_DrawFont_GBK16(0,96, BLUE,GRAY0," DAC VOL:0.000V");	      
	Gui_DrawFont_GBK16(0,112,BLUE,GRAY0," ADC VOL:0.000V");
 	
  DAC_SetChannel1Data(DAC_Align_12b_R,dacval);//��ʼֵΪ0	
	while(1)
	{
		t++;
		
		key=KEY_Scan(0);	//����ɨ��
		
		if(key==WKUP_PRES)//������������DAC���ֵ
		{		
      key=0;//�����ֵ			
			if(dacval<4000)dacval+=200;
			DAC_SetChannel1Data(DAC_Align_12b_R, dacval);//����DACֵ
		}
	 
		
		if(t==10||key==WKUP_PRES) 	//K1������,���߶�ʱʱ�䵽��
		{	  
 			adcx=DAC_GetDataOutputValue(DAC_Channel_1);//��ȡǰ������DAC��ֵ
			
			
			sprintf((char*)tbuf," DAC VAL:0x%x",adcx); //��ʾADCC�������ԭʼֵ
		  Gui_DrawFont_GBK16(4,80,BLUE,GRAY0," DAC VAL:            ");	//����ϴε���ʾ
		  Gui_DrawFont_GBK16(4,80,BLUE,GRAY0,tbuf);	//Һ������ʾ�����
			
			
			temp=(float)adcx*(3.3/4096);			         //�õ�DAC��ѹֵ
			
			sprintf((char*)tbuf," DAC VOL: %5.3f",temp); //��ʾ������
		  Gui_DrawFont_GBK16(4,96,BLUE,GRAY0," DAC VOL:            ");	//����ϴε���ʾ
		  Gui_DrawFont_GBK16(4,96,BLUE,GRAY0,tbuf);	//Һ������ʾ�����
			
			
 			adcx=Get_Adc_Average(ADC_Channel_5,10);		//�õ�ADCת��ֵ	  
			temp=(float)adcx*(3.3/4096);			        //�õ�ADC��ѹֵ
			
			sprintf((char*)tbuf," ADC VOL: %5.3f",temp); //��ʾ������
		  Gui_DrawFont_GBK16(4,112,BLUE,GRAY0," ADC VOL:            ");	//����ϴε���ʾ
		  Gui_DrawFont_GBK16(4,112,BLUE,GRAY0,tbuf);	//Һ������ʾ�����			

			
			LED2=!LED2;	   
			t=0;
		}	    
		delay_ms(10);	 
	}	
}

