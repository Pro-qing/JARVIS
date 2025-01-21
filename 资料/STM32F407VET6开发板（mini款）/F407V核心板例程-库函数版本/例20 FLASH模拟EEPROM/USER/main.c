#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "GUI.h"
#include "Lcd_Driver.h"
#include "stmflash.h" 
#include "key.h"  
//STM32F407������ 
//FLASHģ��EEPROM ʵ�� -�⺯���汾


//Ҫд�뵽STM32 FLASH���ַ�������
const u8 TEXT_Buffer[]={"STM32 FLASH TEST"};
#define TEXT_LENTH sizeof(TEXT_Buffer)	 		  	//���鳤��	
#define SIZE TEXT_LENTH/4+((TEXT_LENTH%4)?1:0)

#define FLASH_SAVE_ADDR  0X0800C004 	//����FLASH �����ַ(����Ϊż��������������,Ҫ���ڱ�������ռ�õ�������.
										//����,д������ʱ��,���ܻᵼ�²�����������,�Ӷ����𲿷ֳ���ʧ.��������.

u8 tbuf[40];//��ʾ������

int main(void)
{ 
	u8 key=0;
	u16 i=0;
	u8 datatemp[SIZE];	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	
	LED_Init();					//��ʼ��LED 
 	
 	KEY_Init();					//������ʼ�� 

	Lcd_Init();	 //1.8��Һ����--��ʼ������
	Lcd_Clear(GRAY0);//����
	
	Gui_DrawFont_GBK16(0,16,RED,GRAY0," STM32F4xx ����");
	Gui_DrawFont_GBK16(0,32,RED,GRAY0," FLASH-EEPROM ");	
	Gui_DrawFont_GBK16(0,48,RED,GRAY0," 2018/5/5");
	Gui_DrawFont_GBK16(0,64,RED,GRAY0," K1:Write&Read ");	//��ʾ��ʾ��Ϣ	
	
	while(1)
	{
		key=KEY_Scan(0);
		if(key==WKUP_PRES)	//K1����,д��STM32 FLASH
		{
			key=0;//���ֵ
 			Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"Start Write FLASH....");
			Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,"                     ");
			STMFLASH_Write(FLASH_SAVE_ADDR,(u32*)TEXT_Buffer,SIZE);
			Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"FLASH Write Finished!");//��ʾ�������
			
			Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"Start Read FLASH.... ");
			STMFLASH_Read(FLASH_SAVE_ADDR,(u32*)datatemp,SIZE);
			Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"The Data Readed Is:  ");//��ʾ�������
			
			sprintf((char*)tbuf,&datatemp[0]); 
			Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,tbuf);					//��ʾ�������ַ���
		}
		i++;
		delay_ms(10);  
		if(i==20)
		{
			LED2=!LED2;//��ʾϵͳ��������	
			i=0;
		}		   
	}    
}

