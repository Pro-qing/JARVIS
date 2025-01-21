#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"


//STM32F407������ 
//���벶��ʵ�� -�⺯���汾

 
 
 
extern u8  TIM5CH1_CAPTURE_STA;		//���벶��״̬		    				
extern u32	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ  
  
	
int main(void)
{ 
	long long temp=0;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);//��ʼ�����ڲ�����Ϊ115200
	
	printf("��ʱ������ʵ�飺��������ʱ��\r\n"); //��ӡ�ܵĸߵ�ƽʱ��
 
 	TIM5_CH1_Cap_Init(0XFFFFFFFF,84-1); //��1Mhz��Ƶ�ʼ��� 
   	while(1)
	{
 		delay_ms(10);
			 
 		if(TIM5CH1_CAPTURE_STA&0X80)        //�ɹ�������һ�θߵ�ƽ
		{
			temp=TIM5CH1_CAPTURE_STA&0X3F; 
			temp*=0XFFFFFFFF;		 		         //���ʱ���ܺ�
			temp+=TIM5CH1_CAPTURE_VAL;		   //�õ��ܵĸߵ�ƽʱ��
			printf("HIGH:%lld us\r\n",temp); //��ӡ�ܵĸߵ�ƽʱ��
			TIM5CH1_CAPTURE_STA=0;			     //������һ�β���
		}
	}
}
