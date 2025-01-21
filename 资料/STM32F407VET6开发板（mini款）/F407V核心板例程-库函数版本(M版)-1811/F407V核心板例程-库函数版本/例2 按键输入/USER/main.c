#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"


//STM32F407开发板 
//按键输入实验-库函数版本 



int main(void)
{ 
 
  u8 key;           //保存键值
	delay_init(168);  //初始化延时函数
	LED_Init();				//初始化LED端口 
	
	KEY_Init();       //初始化与按键连接的硬件接口
	LED2=0;				  	//先点亮LED灯
	while(1)
	{
		key=KEY_Scan(0);		//得到键值
	  if(key)
		{						   
			switch(key)
			{				 
				case WKUP_PRES:	//按键K1按下
					
					LED2=! LED2;//LED状态翻转
					break;

			}
		}else delay_ms(10); 
	}

}
