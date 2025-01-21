
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

//STM32F407开发板 
//LED闪 实验 -库函数版本


int main(void)
{ 
 
	delay_init(168);		  //初始化延时函数
	LED_Init();		        //初始化LED端口
	
  /**下面是通过直接操作库函数的方式实现IO控制**/	
	
	while(1)
	{
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);  //LED0对应引脚GPIOF.9拉低，亮  等同LED2=0;
	
	delay_ms(500);  		   //延时500ms
	GPIO_SetBits(GPIOA,GPIO_Pin_1);	   //LED0对应引脚GPIOF.0拉高，灭  等同LED2=1;
	
	delay_ms(500);                     //延时500ms
	}
}



 



