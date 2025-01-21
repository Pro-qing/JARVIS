#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"


//STM32F407������ 
//��������ʵ��-�⺯���汾 



int main(void)
{ 
 
  u8 key;           //�����ֵ
	delay_init(168);  //��ʼ����ʱ����
	LED_Init();				//��ʼ��LED�˿� 
	
	KEY_Init();       //��ʼ���밴�����ӵ�Ӳ���ӿ�
	LED2=0;				  	//�ȵ���LED��
	while(1)
	{
		key=KEY_Scan(0);		//�õ���ֵ
	  if(key)
		{						   
			switch(key)
			{				 
				case WKUP_PRES:	//����K1����
					
					LED2=! LED2;//LED״̬��ת
					break;

			}
		}else delay_ms(10); 
	}

}
