
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

#include "rng.h"
#include "key.h"

#include "GUI.h"
#include "Lcd_Driver.h"

//STM32F407开发板 
//随机数发生器(RNG)  -库函数版本

  u32 random;
	u8 t=0,key;
  u8 tbuf[40];	
    
int main(void)
{ 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	LED_Init();					//初始化LED
	KEY_Init();					//按键初始化
	
	Lcd_Init();	 //1.8寸液晶屏--初始化配置
	Lcd_Clear(GRAY0);//清屏
	
	Gui_DrawFont_GBK16(0,16,RED,GRAY0," STM32F4xx 测试");
	Gui_DrawFont_GBK16(0,32,RED,GRAY0,"RNG TEST");	
	Gui_DrawFont_GBK16(0,48,RED,GRAY0,"2018/5/5");	 
	
	
	while(RNG_Init())	 		//初始化随机数发生器
	{
		Gui_DrawFont_GBK16(0,64,RED,GRAY0,"  RNG Error! ");	 
		delay_ms(200);
		Gui_DrawFont_GBK16(0,80,RED,GRAY0,"RNG Trying...");	 
	} 
	
	Gui_DrawFont_GBK16(0,32,RED,GRAY0,"RNG Ready!   ");	 
	Gui_DrawFont_GBK16(0,48,RED,GRAY0,"KEY0:Get Random");	 
	Gui_DrawFont_GBK16(0,64,RED,GRAY0,"Random Num:");	 
	Gui_DrawFont_GBK16(0,80,RED,GRAY0,"Random Num[0-9]:");	 	
  

	while(1) 
	{		
		delay_ms(10);
		key=KEY_Scan(0);
		
		if(key==WKUP_PRES)
		{
			
			random=RNG_Get_RandomNum(); //获得随机数
			sprintf((char*)tbuf,"Random:%d",random); //显示随机数
			Gui_DrawFont_GBK16(4,96,BLUE,GRAY0,tbuf);	//液晶屏显示随机数
			
      printf("\n\r Random:%d",random); //串口显示随机数
		} 
		if((t%20)==0)
		{ 
			LED2=!LED2;	//每200ms,翻转一次LED0 
			random=RNG_Get_RandomRange(0,9);//获取[0,9]区间的随机数
			sprintf((char*)tbuf,"Random:%d",random); //显示随机数
			Gui_DrawFont_GBK16(4,96,BLUE,GRAY0,tbuf);	//液晶屏显示随机数
			
			printf("\n\r Random:%d",random); //串口显示随机数
			
		 }
		delay_ms(10);
		t++;
	}	
}

