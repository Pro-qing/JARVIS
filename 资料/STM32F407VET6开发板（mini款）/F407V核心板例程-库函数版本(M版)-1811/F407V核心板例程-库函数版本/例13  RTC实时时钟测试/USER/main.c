#include "sys.h"
#include "delay.h"  
#include "usart.h"  
#include "led.h"

#include "rtc.h"

#include "GUI.h"
#include "Lcd_Driver.h"

//STM32F407开发板 
//RTC实时时钟 实验 -库函数版本


	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	
	u8 tbuf[40];
	u8 t=0;
	u8 Time_S;//秒记录
int main(void)
{ 


	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);      //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	
	Lcd_Init();	 //1.8寸液晶屏--初始化配置
	Lcd_Clear(GRAY0);//清屏
	Gui_DrawFont_GBK16(0,16,RED,GRAY0," STM32F4xx 测试");
	
	printf("\r\n STM32F4xx核心板\r\n");
	printf("\r\n 测试程序例程\r\n");
	
	LED_Init();					  //初始化LED
 	
	My_RTC_Init();		 		//初始化RTC
 
	RTC_Set_WakeUp(RTC_WakeUpClock_CK_SPRE_16bits,0);		//配置WAKE UP中断,1秒钟中断一次
	
	
  while(1) 
	{		
		t++;
		if((t%30)==0)	//每300ms更新一次显示数据
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
		if(Time_S!=RTC_TimeStruct.RTC_Seconds)//每秒钟串口输出一次秒状态
			{
				Time_S=RTC_TimeStruct.RTC_Seconds;
				LED2=!LED2; ///每秒钟,翻转一次LED2
			  printf("\n\r Time:%02d:%02d:%02d",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds);
		  }
		
		if(t>200)t=0;//复位计数值
		delay_ms(10);
	}	
}
