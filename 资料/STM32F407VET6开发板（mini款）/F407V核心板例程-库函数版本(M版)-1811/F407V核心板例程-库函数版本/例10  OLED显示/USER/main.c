
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "oled.h"


//探索者STM32F407开发板 
//OLED显示实验-库函数版本 



int main(void)
{ 
	u8 t=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);     //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	
	printf("\r\n STM32F407开发板 OLED显示测试 \r\n");
	
	LED_Init();					//初始化LED
 	OLED_Init();				//初始化OLED
  OLED_ShowString(0,0,"mcudev",24);  
	OLED_ShowString(0,24, "0.96' OLED TEST",16);  
 	OLED_ShowString(0,40,"Date: 2018/5/4",12);  
 	OLED_ShowString(0,52,"ASCII:",12);  
 	OLED_ShowString(64,52,"CODE:",12);
  
	OLED_Refresh_Gram();//更新显示到OLED	 
	
	t=' ';  
	while(1) 
	{		
		OLED_ShowChar(36,52,t,12,1);//显示ASCII字符	
		OLED_ShowNum(94,52,t,3,12);	//显示ASCII字符的码值    
		OLED_Refresh_Gram();        //更新显示到OLED
		t++;
		if(t>'~')t=' ';  
		delay_ms(500);
		LED2=!LED2;
	}
}
