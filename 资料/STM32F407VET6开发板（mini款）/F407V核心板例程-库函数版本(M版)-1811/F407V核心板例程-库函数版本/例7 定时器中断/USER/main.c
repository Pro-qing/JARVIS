#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"


//STM32F407������ 
//��ʱ���ж�ʵ��-�⺯���汾

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	LED_Init();				//��ʼ��LED�˿�
  uart_init(115200);	//���ڳ�ʼ��������Ϊ115200
	
	
 	TIM3_Int_Init(5000-1,8400-1);	//��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����5000��Ϊ500ms     
	while(1)
	{
		printf("\r\n ��ʱ���ж�ʵ��:LED���ж��з�ת״̬��\r\n");//D2��ת
		delay_ms(500);//��ʱ500ms
	};
}
