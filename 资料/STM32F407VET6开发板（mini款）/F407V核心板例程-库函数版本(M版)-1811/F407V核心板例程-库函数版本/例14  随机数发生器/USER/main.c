
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

#include "rng.h"
#include "key.h"

#include "GUI.h"
#include "Lcd_Driver.h"

//STM32F407������ 
//�����������(RNG)  -�⺯���汾

  u32 random;
	u8 t=0,key;
  u8 tbuf[40];	
    
int main(void)
{ 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED
	KEY_Init();					//������ʼ��
	
	Lcd_Init();	 //1.8��Һ����--��ʼ������
	Lcd_Clear(GRAY0);//����
	
	Gui_DrawFont_GBK16(0,16,RED,GRAY0," STM32F4xx ����");
	Gui_DrawFont_GBK16(0,32,RED,GRAY0,"RNG TEST");	
	Gui_DrawFont_GBK16(0,48,RED,GRAY0,"2018/5/5");	 
	
	
	while(RNG_Init())	 		//��ʼ�������������
	{
		Gui_DrawFont_GBK16(0,64,RED,GRAY0,"  RNG Error! ");	 
		delay_ms(200);
		Gui_DrawFont_GBK16(0,80,RED,GRAY0,"RNG Trying...");	 
	} 
	
	Gui_DrawFont_GBK16(0,32,RED,GRAY0,"RNG Ready!   ");	 
	Gui_DrawFont_GBK16(0,48,RED,GRAY0,"KEY0:Get Random");	 
	Gui_DrawFont_GBK16(0,64,RED,GRAY0,"Random Num:");	 
	Gui_DrawFont_GBK16(0,80,RED,GRAY0,"Random Num[0-9]:");	 	
  

	while(1) 
	{		
		delay_ms(10);
		key=KEY_Scan(0);
		
		if(key==WKUP_PRES)
		{
			
			random=RNG_Get_RandomNum(); //��������
			sprintf((char*)tbuf,"Random:%d",random); //��ʾ�����
			Gui_DrawFont_GBK16(4,96,BLUE,GRAY0,tbuf);	//Һ������ʾ�����
			
      printf("\n\r Random:%d",random); //������ʾ�����
		} 
		if((t%20)==0)
		{ 
			LED2=!LED2;	//ÿ200ms,��תһ��LED0 
			random=RNG_Get_RandomRange(0,9);//��ȡ[0,9]����������
			sprintf((char*)tbuf,"Random:%d",random); //��ʾ�����
			Gui_DrawFont_GBK16(4,96,BLUE,GRAY0,tbuf);	//Һ������ʾ�����
			
			printf("\n\r Random:%d",random); //������ʾ�����
			
		 }
		delay_ms(10);
		t++;
	}	
}

