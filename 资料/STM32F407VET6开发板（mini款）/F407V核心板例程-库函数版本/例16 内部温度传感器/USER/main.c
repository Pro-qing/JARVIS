#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "GUI.h"
#include "Lcd_Driver.h"
#include "adc.h"


//STM32F407������ 
//�ڲ��¶ȴ�����ʵ�� -�⺯���汾 

	float temp;     
  u8 tbuf[40];	//��ʾ������		
int main(void)
{ 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);     //��ʼ����ʱ����
	uart_init(115200);	 //��ʼ�����ڲ�����Ϊ115200

	LED_Init();					//��ʼ��LED 
 
	Lcd_Init();	 //1.8��Һ����--��ʼ������
	Lcd_Clear(GRAY0);//����
	
	Gui_DrawFont_GBK16(0,16,RED,GRAY0," STM32F4xx ����");
	Gui_DrawFont_GBK16(0,32,RED,GRAY0,"Temperature TEST");	
	Gui_DrawFont_GBK16(0,48,RED,GRAY0,"2018/5/5");	
	
	Adc_Init();         //�ڲ��¶ȴ�����ADC��ʼ��
	
	
	Gui_DrawFont_GBK16(4,80,BLUE,GRAY0,"Tem: 00.00C");//���ڹ̶�λ����ʾС����	      
	while(1)
	{
		temp=Get_Temprate();	//�õ��¶�ֵ 

		temp=temp/100;
		
		sprintf((char*)tbuf,"TEMP:%06.3f C",temp); 
		
		printf("\r\n CPU�¶�:%06.3fC",temp);//���ڴ�ӡ���
		
		Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"TEMP:       C");
		Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,tbuf);
    
		 
		LED2=!LED2;
		delay_ms(500);	
	}
}
