
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

//STM32F407������ 
//LED�� ʵ�� -�⺯���汾


int main(void)
{ 
 
	delay_init(168);		  //��ʼ����ʱ����
	LED_Init();		        //��ʼ��LED�˿�
	
  /**������ͨ��ֱ�Ӳ����⺯���ķ�ʽʵ��IO����**/	
	
	while(1)
	{
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);  //LED0��Ӧ����GPIOF.9���ͣ���  ��ͬLED2=0;
	
	delay_ms(500);  		   //��ʱ500ms
	GPIO_SetBits(GPIOA,GPIO_Pin_1);	   //LED0��Ӧ����GPIOF.0���ߣ���  ��ͬLED2=1;
	
	delay_ms(500);                     //��ʱ500ms
	}
}



 



