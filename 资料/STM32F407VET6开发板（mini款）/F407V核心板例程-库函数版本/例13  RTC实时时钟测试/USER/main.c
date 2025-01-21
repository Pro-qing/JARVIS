#include "sys.h"
#include "delay.h"  
#include "usart.h"  
#include "led.h"

#include "rtc.h"

#include "GUI.h"
#include "Lcd_Driver.h"

//STM32F407������ 
//RTCʵʱʱ�� ʵ�� -�⺯���汾


	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	
	u8 tbuf[40];
	u8 t=0;
	u8 Time_S;//���¼
int main(void)
{ 


	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);      //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	
	Lcd_Init();	 //1.8��Һ����--��ʼ������
	Lcd_Clear(GRAY0);//����
	Gui_DrawFont_GBK16(0,16,RED,GRAY0," STM32F4xx ����");
	
	printf("\r\n STM32F4xx���İ�\r\n");
	printf("\r\n ���Գ�������\r\n");
	
	LED_Init();					  //��ʼ��LED
 	
	My_RTC_Init();		 		//��ʼ��RTC
 
	RTC_Set_WakeUp(RTC_WakeUpClock_CK_SPRE_16bits,0);		//����WAKE UP�ж�,1�����ж�һ��
	
	
  while(1) 
	{		
		t++;
		if((t%30)==0)	//ÿ300ms����һ����ʾ����
		{
			RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
			
			sprintf((char*)tbuf,"Time:%02d:%02d:%02d",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds); 
			
			Gui_DrawFont_GBK16(4,48,BLUE,GRAY0,tbuf);	
			
			RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
			
			sprintf((char*)tbuf,"Date:20%02d-%02d-%02d",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date); 
			Gui_DrawFont_GBK16(4,64,BLUE,GRAY0,tbuf);	
			sprintf((char*)tbuf,"Week:%d",RTC_DateStruct.RTC_WeekDay); 
			Gui_DrawFont_GBK16(4,80,BLUE,GRAY0,tbuf);
			
		} 
		if(Time_S!=RTC_TimeStruct.RTC_Seconds)//ÿ���Ӵ������һ����״̬
			{
				Time_S=RTC_TimeStruct.RTC_Seconds;
				LED2=!LED2; ///ÿ����,��תһ��LED2
			  printf("\n\r Time:%02d:%02d:%02d",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds);
		  }
		
		if(t>200)t=0;//��λ����ֵ
		delay_ms(10);
	}	
}
