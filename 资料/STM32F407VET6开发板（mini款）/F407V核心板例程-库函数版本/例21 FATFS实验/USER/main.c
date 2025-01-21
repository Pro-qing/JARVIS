#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"


#include "GUI.h"
#include "Lcd_Driver.h"

#include "key.h"  
  
#include "malloc.h" 
  
#include "sdio_sdcard.h"    
#include "malloc.h" 
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"



//STM32F407������ 
//FATFS ʵ�� -�⺯���汾

 u8 tbuf[40];	//��ʾ������

int main(void)
{        
 	u32 total,free;
	u8 t=0;	
	u8 res=0;	

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED 
	 
 	KEY_Init();					//������ʼ�� 
	W25QXX_Init();				//��ʼ��W25Q16
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ�� 
	my_mem_init(SRAMCCM);		//��ʼ��CCM�ڴ��
	
	Lcd_Init();	 //1.8��Һ����--��ʼ������
	Lcd_Clear(GRAY0);//����
	
	Gui_DrawFont_GBK16(0,16,RED,GRAY0," STM32F4xx ����");
	Gui_DrawFont_GBK16(0,32,RED,GRAY0," FATFS TEST");	
	Gui_DrawFont_GBK16(0,48,RED,GRAY0," 2018/5/5");
	
	
	   
 	while(SD_Init())//��ⲻ��SD��
	{
		Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"SD Card Error!");
		delay_ms(500);					
		Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,"Please Check! ");
		delay_ms(500);
		LED2=!LED2;//DS0��˸
	}
	
 	exfuns_init();							//Ϊfatfs��ر��������ڴ�				 
  f_mount(fs[0],"0:",1); 					//����SD�� 
 	res=f_mount(fs[1],"1:",1); 				//����FLASH.	
	
	
	if(res==0X0D)//FLASH����,FAT�ļ�ϵͳ����,���¸�ʽ��FLASH
	{
		Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"Flash Disk Formatting...");	//��ʽ��FLASH
		res=f_mkfs("1:",1,4096);//��ʽ��FLASH,1,�̷�;1,����Ҫ������,8������Ϊ1����
		if(res==0)
		{
			f_setlabel((const TCHAR *)"1:mcudev");	//����Flash���̵�����Ϊ��mcudev
			Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"Flash Disk Format Finish");	//��ʽ�����
		}
    else Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"Flash Disk Format Error ");	//��ʽ��ʧ��
		delay_ms(1000);
	}	

	
	Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"                    ");;	//�����ʾ	
				  
	while(exf_getfree("0",&total,&free))	//�õ�SD������������ʣ������
	{
		Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"SD Card Fatfs Error!");
		delay_ms(200);
		Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"                    ");;	//�����ʾ			  
		delay_ms(200);
		LED2=!LED2;//DS0��˸
	}													  			    
 	
	Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"FATFS OK!");	

	sprintf((char*)tbuf,"SD Total:%d MB",total>>10); //��ʾSD�������� MB
	Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,tbuf);
	sprintf((char*)tbuf,"SD Free:%d MB",free>>10); //��ʾSD��ʣ������ MB		
	Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,tbuf);	
	
//	LCD_ShowString(30,170,200,16,16,"SD Total Size:     MB");	 
//	LCD_ShowString(30,190,200,16,16,"SD  Free Size:     MB"); 	    
// 	LCD_ShowNum(30+8*14,170,total>>10,5,16);				//��ʾSD�������� MB
// 	LCD_ShowNum(30+8*14,190,free>>10,5,16);					//��ʾSD��ʣ������ MB			

	
	while(1)
	{
		t++; 
		delay_ms(200);		 			   
		LED2=!LED2;
	} 
}





