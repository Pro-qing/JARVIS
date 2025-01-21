
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

#include "GUI.h"
#include "Lcd_Driver.h"


//STM32F407开发板 
//1.44寸液晶屏--显示实验-库函数版本


int main(void)
{ 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);      //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	
	printf("\r\n STM32F4xx核心板\r\n");
	printf("\r\n 测试程序例程\r\n");
	
	LED_Init();					  //初始化LED
	
	Lcd_Init();	 //1.8寸液晶屏--初始化配置
	Lcd_Clear(GRAY0);//清屏
	Gui_DrawFont_GBK16(0,16,RED,GRAY0,"   STM32F4xx ");
  Gui_DrawFont_GBK16(0,32,BLUE,GRAY0," 液晶测试程序");	 
	Gui_DrawFont_GBK16(0,48,BLUE,GRAY0,"mcudev.taobao.com ");
	
  while(1) 
	{		 
     	Test_Demo();	//液晶屏测试例程
			
			printf("\r\n STM32F4xx核心板\r\n");
			printf("\r\n 测试程序例程\r\n");
		
	} 
}
