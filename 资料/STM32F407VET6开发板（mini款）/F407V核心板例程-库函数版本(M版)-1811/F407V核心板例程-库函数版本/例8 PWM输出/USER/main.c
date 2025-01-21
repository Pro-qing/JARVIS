#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "pwm.h"


//STM32F407开发板 
//PWM输出实验-库函数版本

	u16 pwmval=0;    
	u8 dir=1;
	
int main(void)
{ 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);//初始化串口波特率为115200
	
 	LED_Init();//初始化LED
	
	LED2=0;
	
	printf("\r\n定时器PWM输出实验:运行。\r\n");
	
	TIM3_PWM_Init(500-1,84-1);	//84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz.     
  
	while(1) //实现比较值从0-300递增，到300后从300-0递减，循环
	{
 		delay_ms(10);	 
		if(dir)pwmval++;//dir==1 led0pwmval递增
		else pwmval--;	//dir==0 led0pwmval递减 
 		if(pwmval>300)dir=0;//led0pwmval到达300后，方向为递减
		if(pwmval==0)dir=1;	//led0pwmval递减到0后，方向改为递增
 
		TIM_SetCompare1(TIM3,pwmval);	//修改比较值，修改占空比
	}
}
