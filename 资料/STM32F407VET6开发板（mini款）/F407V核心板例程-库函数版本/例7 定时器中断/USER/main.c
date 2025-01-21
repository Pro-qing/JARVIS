#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"


//STM32F407开发板 
//定时器中断实验-库函数版本

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	LED_Init();				//初始化LED端口
  uart_init(115200);	//串口初始化波特率为115200
	
	
 	TIM3_Int_Init(5000-1,8400-1);	//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数5000次为500ms     
	while(1)
	{
		printf("\r\n 定时器中断实验:LED在中断中翻转状态。\r\n");//D2翻转
		delay_ms(500);//延时500ms
	};
}
