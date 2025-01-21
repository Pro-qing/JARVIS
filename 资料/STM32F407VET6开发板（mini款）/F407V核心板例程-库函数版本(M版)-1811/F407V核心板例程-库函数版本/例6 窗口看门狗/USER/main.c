#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

#include "key.h"
#include "wwdg.h"


//STM32F407������ 
//���ڿ��Ź�ʵ�� -�⺯���汾


int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	
	LED_Init();				//��ʼ��LED�˿�
	uart_init(115200);	//���ڳ�ʼ��������Ϊ115200
	KEY_Init();		  	//��ʼ������
	LED2=0;				   //����LED2
	
	delay_ms(300);
	WWDG_Init(0x7F,0X5F,WWDG_Prescaler_8); 	//������ֵΪ7f,���ڼĴ���Ϊ5f,��Ƶ��Ϊ8	   
	
	while(1)
	{
		printf("\r\n ���ڿ��Ź�ʵ��: �����С�����\r\n");
		delay_ms(300);
	}
}
