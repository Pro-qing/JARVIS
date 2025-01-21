#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "GUI.h"
#include "Lcd_Driver.h"
#include "key.h"  
#include "malloc.h" 
#include "sdio_sdcard.h"    
//STM32F407������ 
//SD�� ʵ�� -�⺯���汾



//ͨ�����ڴ�ӡSD�������Ϣ
void show_sdcard_info(void)
{
	switch(SDCardInfo.CardType)
	{
		case SDIO_STD_CAPACITY_SD_CARD_V1_1:printf("Card Type:SDSC V1.1\r\n");break;
		case SDIO_STD_CAPACITY_SD_CARD_V2_0:printf("Card Type:SDSC V2.0\r\n");break;
		case SDIO_HIGH_CAPACITY_SD_CARD:printf("Card Type:SDHC V2.0\r\n");break;
		case SDIO_MULTIMEDIA_CARD:printf("Card Type:MMC Card\r\n");break;
	}	
  	printf("Card ManufacturerID:%d\r\n",SDCardInfo.SD_cid.ManufacturerID);	//������ID
 	printf("Card RCA:%d\r\n",SDCardInfo.RCA);								//����Ե�ַ
	printf("Card Capacity:%d MB\r\n",(u32)(SDCardInfo.CardCapacity>>20));	//��ʾ����
 	printf("Card BlockSize:%d\r\n\r\n",SDCardInfo.CardBlockSize);			//��ʾ���С
}


 u8 tbuf[40];	//��ʾ������


int main(void)
{        
	u8 key;		 
	u32 sd_size;
	u8 t=0;	
	u8 *buf;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED  
 	
 	KEY_Init();					//������ʼ��  
	
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ�� 
	my_mem_init(SRAMCCM);		//��ʼ��CCM�ڴ��
	
	Lcd_Init();	 //1.8��Һ����--��ʼ������
	Lcd_Clear(GRAY0);//����
	
	Gui_DrawFont_GBK16(0,16,RED,GRAY0," STM32F4xx ����");
	Gui_DrawFont_GBK16(0,32,RED,GRAY0," SD Card TEST");	
	Gui_DrawFont_GBK16(0,48,RED,GRAY0," 2018/5/5");
	Gui_DrawFont_GBK16(0,64,RED,GRAY0," K1:Write&Read ");	//��ʾ��ʾ��Ϣ	
  
   
 	while(SD_Init())//��ⲻ��SD��
	{
		Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"SD Card Error!");
		delay_ms(500);					
		Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,"Please Check! ");
		delay_ms(500);
		LED2=!LED2;//DS0��˸
	}
	show_sdcard_info();	//��ӡSD�������Ϣ
 	
	//���SD���ɹ� 											    
	Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"SD Card OK    ");
	
	
	sprintf((char*)tbuf,"SD Size:%d MB",SDCardInfo.CardCapacity>>20); //��ʾSD������
	Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,tbuf);
	
	
	
	while(1)
	{
		key=KEY_Scan(0);
		if(key==WKUP_PRES)//K1������
		{
			key=0;//�����ֵ
			
			buf=mymalloc(0,512);		//�����ڴ�
			
			if(SD_ReadDisk(buf,0,1)==0)	//��ȡ0����������
			{	
				Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,"USART1 Sending Data...");
				printf("SECTOR 0 DATA:\r\n");
				for(sd_size=0;sd_size<512;sd_size++)printf("%x ",buf[sd_size]);//��ӡ0��������    	   
				printf("\r\nDATA ENDED\r\n");
				Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,"USART1 Send Data Over!");
			}
			myfree(0,buf);//�ͷ��ڴ�	   
		}   
		t++;
		delay_ms(10);
		if(t==20)
		{
			LED2=!LED2;
			t=0;
		}
	} }





