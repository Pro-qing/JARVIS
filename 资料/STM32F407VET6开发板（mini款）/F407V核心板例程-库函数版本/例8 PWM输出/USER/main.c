#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "pwm.h"


//STM32F407������ 
//PWM���ʵ��-�⺯���汾

	u16 pwmval=0;    
	u8 dir=1;
	
int main(void)
{ 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);//��ʼ�����ڲ�����Ϊ115200
	
 	LED_Init();//��ʼ��LED
	
	LED2=0;
	
	printf("\r\n��ʱ��PWM���ʵ��:���С�\r\n");
	
	TIM3_PWM_Init(500-1,84-1);	//84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz.     
  
	while(1) //ʵ�ֱȽ�ֵ��0-300��������300���300-0�ݼ���ѭ��
	{
 		delay_ms(10);	 
		if(dir)pwmval++;//dir==1 led0pwmval����
		else pwmval--;	//dir==0 led0pwmval�ݼ� 
 		if(pwmval>300)dir=0;//led0pwmval����300�󣬷���Ϊ�ݼ�
		if(pwmval==0)dir=1;	//led0pwmval�ݼ���0�󣬷����Ϊ����
 
		TIM_SetCompare1(TIM3,pwmval);	//�޸ıȽ�ֵ���޸�ռ�ձ�
	}
}
