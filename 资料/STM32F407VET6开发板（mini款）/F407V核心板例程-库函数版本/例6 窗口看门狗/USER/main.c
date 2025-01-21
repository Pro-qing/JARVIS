#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

#include "key.h"
#include "wwdg.h"


//STM32F407开发板 
//窗口看门狗实验 -库函数版本


int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	
	LED_Init();				//初始化LED端口
	uart_init(115200);	//串口初始化波特率为115200
	KEY_Init();		  	//初始化按键
	LED2=0;				   //点亮LED2
	
	delay_ms(300);
	WWDG_Init(0x7F,0X5F,WWDG_Prescaler_8); 	//计数器值为7f,窗口寄存器为5f,分频数为8	   
	
	while(1)
	{
		printf("\r\n 窗口看门狗实验: 运行中。。。\r\n");
		delay_ms(300);
	}
}
