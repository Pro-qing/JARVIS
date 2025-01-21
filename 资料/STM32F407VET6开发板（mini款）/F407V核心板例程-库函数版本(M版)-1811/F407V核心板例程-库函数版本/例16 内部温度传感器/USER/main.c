#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "GUI.h"
#include "Lcd_Driver.h"
#include "adc.h"


//STM32F407开发板 
//内部温度传感器实验 -库函数版本 

	float temp;     
  u8 tbuf[40];	//显示缓冲区		
int main(void)
{ 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);     //初始化延时函数
	uart_init(115200);	 //初始化串口波特率为115200

	LED_Init();					//初始化LED 
 
	Lcd_Init();	 //1.8寸液晶屏--初始化配置
	Lcd_Clear(GRAY0);//清屏
	
	Gui_DrawFont_GBK16(0,16,RED,GRAY0," STM32F4xx 测试");
	Gui_DrawFont_GBK16(0,32,RED,GRAY0,"Temperature TEST");	
	Gui_DrawFont_GBK16(0,48,RED,GRAY0,"2018/5/5");	
	
	Adc_Init();         //内部温度传感器ADC初始化
	
	
	Gui_DrawFont_GBK16(4,80,BLUE,GRAY0,"Tem: 00.00C");//先在固定位置显示小数点	      
	while(1)
	{
		temp=Get_Temprate();	//得到温度值 

		temp=temp/100;
		
		sprintf((char*)tbuf,"TEMP:%06.3f C",temp); 
		
		printf("\r\n CPU温度:%06.3fC",temp);//串口打印结果
		
		Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"TEMP:       C");
		Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,tbuf);
    
		 
		LED2=!LED2;
		delay_ms(500);	
	}
}
