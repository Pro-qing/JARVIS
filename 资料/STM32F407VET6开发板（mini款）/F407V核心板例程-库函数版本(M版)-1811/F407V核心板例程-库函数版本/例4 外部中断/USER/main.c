#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

#include "key.h"
#include "exti.h"

//STM32F407������ 
//�ⲿ�ж�ʵ��-�⺯���汾


int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200); 	//���ڳ�ʼ�� 
	LED_Init();				  //��ʼ��LED�˿�  
	
	EXTIX_Init();       //��ʼ���ⲿ�ж����� 
	LED2=0;					    //�ȵ���LED
	while(1)
	{
  	printf("OK\r\n");	//��ӡOK��ʾ��������
		delay_ms(1000);	  //ÿ��1s��ӡһ��
	}

}
