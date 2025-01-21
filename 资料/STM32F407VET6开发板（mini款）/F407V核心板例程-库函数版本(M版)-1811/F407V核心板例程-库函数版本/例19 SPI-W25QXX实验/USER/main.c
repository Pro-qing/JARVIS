#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "GUI.h"
#include "Lcd_Driver.h"
#include "spi.h"
#include "w25qxx.h"
#include "key.h"  


//STM32F407������ 
//SPIͨ�Žӿ�ʵ��-�⺯���汾
 
 
//Ҫд�뵽W25Q16���ַ�������
const u8 TEXT_Buffer[]={"mcudev STM32F4"};
#define SIZE sizeof(TEXT_Buffer)	 

u8 tbuf[40];//��ʾ�ַ�ת����


int main(void)
{ 
	u8 key;
	u16 i=0;
	u8 datatemp[SIZE];
	u32 FLASH_SIZE;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);     //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED 
 	
	
	KEY_Init(); 				//������ʼ��  
	W25QXX_Init();			//W25QXX��ʼ��

	Lcd_Init();	 //1.8��Һ����--��ʼ������
	Lcd_Clear(GRAY0);//����
	
	Gui_DrawFont_GBK16(0,16,RED,GRAY0," STM32F4xx ����");
	Gui_DrawFont_GBK16(0,32,RED,GRAY0," SPI W25QXX TEST");	
	Gui_DrawFont_GBK16(0,48,RED,GRAY0," 2018/5/5");
	Gui_DrawFont_GBK16(0,64,RED,GRAY0," K1:Write&Read ");	//��ʾ��ʾ��Ϣ		
	
	
	while(W25QXX_ReadID()!=W25Q16)								//��ⲻ��W25Q16
	{
		Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"W25Q16 Check Failed!");
		delay_ms(500);
		Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,"Please Check!      ");
		delay_ms(500);
		LED2=!LED2;		//DS0��˸
	}
	Gui_DrawFont_GBK16(0,80,RED,GRAY0," W25Q16 Ready!"); 
	FLASH_SIZE=2*1024*1024;	//FLASH ��СΪ2�ֽ�
   
	while(1)
	{
		key=KEY_Scan(0);

		if(key==WKUP_PRES)//KEY1����,д��W25Q16
		{
			 
 			Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,"Start Write W25Q16....");
			Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,"                     ");					//��ʾ�������ַ���
			
			W25QXX_Write((u8*)TEXT_Buffer,FLASH_SIZE-100,SIZE);		//�ӵ�����100����ַ����ʼ,д��SIZE���ȵ�����
			Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,"W25Q16 Write Finished!");	//��ʾ�������
			
			Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,"Start Read W25Q16.... ");
			W25QXX_Read(datatemp,FLASH_SIZE-100,SIZE);					//�ӵ�����100����ַ����ʼ,����SIZE���ֽ�
			Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,"The Data Readed Is:   ");	//��ʾ�������
			sprintf((char*)tbuf,&datatemp[0]); 
			Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,tbuf);					//��ʾ�������ַ���
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

