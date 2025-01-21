#include "sys.h"
#include "delay.h"  
#include "usart.h"   
#include "led.h"
#include "GUI.h"
#include "Lcd_Driver.h"
#include "key.h"   
#include "malloc.h" 
#include "w25qxx.h"    
#include "sdio_sdcard.h"    
#include "usbd_msc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usb_conf.h" 

//̽����STM32F407������ 
//USB������ ʵ�� -�⺯���汾

 

USB_OTG_CORE_HANDLE USB_OTG_dev;
extern vu8 USB_STATUS_REG;		//USB״̬
extern vu8 bDeviceState;		//USB���� ���

 u8 tbuf[40];	//��ʾ������

int main(void)
{        
	u8 offline_cnt=0;
	u8 tct=0;
	u8 USB_STA;
	u8 Divece_STA;


	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED  
 	
 	KEY_Init();					//������ʼ��  
	W25QXX_Init();				//��ʼ��W25Q16  
  
	
	Lcd_Init();	 //1.8��Һ����--��ʼ������
	Lcd_Clear(GRAY0);//����
	
	Gui_DrawFont_GBK16(0,16,RED,GRAY0," STM32F4xx ����");
	Gui_DrawFont_GBK16(0,32,RED,GRAY0," USB Card TEST");	
	Gui_DrawFont_GBK16(0,48,RED,GRAY0," 2018/5/5");
	




	if(SD_Init())
		Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"SD Card Error!");	//���SD������
	else //SD ������
	{   															  
		
		sprintf((char*)tbuf,"SD Size:%d MB",SDCardInfo.CardCapacity>>20); //��ʾSD������
	  Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,tbuf);
		
 	}
	
	if(W25QXX_ReadID()!=W25Q16)
		Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,"W25Q16 Error!");	//���W25Q128����
	else //SPI FLASH ����
	{   														 
		Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,"W25Q16 Size:2MB");	
 		
	}  
 	Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,"USB Connecting...");//��ʾ���ڽ������� 	   
	
	USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_MSC_cb,&USR_cb);
	
	delay_ms(1800);	
	
	while(1)
	{	
		delay_ms(5);		
		
		if(USB_STA!=USB_STATUS_REG)//״̬�ı��� 
		{	 						   
			Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,"                ");//�����ʾ			  	   
			if(USB_STATUS_REG&0x01)//����д		  
			{
				LED2=0;
				Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,"USB Writing...");//��ʾUSB����д������	 
			}
			if(USB_STATUS_REG&0x02)//���ڶ�
			{
				LED2=0;
				Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,"USB Reading...");//��ʾUSB���ڶ�������  		 
			}	 										  
			if(USB_STATUS_REG&0x04)
				Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,"USB Write Err ");//��ʾд�����
			else 
				Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,"                ");//�����ʾ		  
			if(USB_STATUS_REG&0x08)
				Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,"USB Read  Err ");//��ʾ��������
			else 
				Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,"                ");//�����ʾ	 
			USB_STA=USB_STATUS_REG;//��¼����״̬
		}
		
		if(Divece_STA!=bDeviceState) 
		{
			if(bDeviceState==1)
				Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,"USB Connected    ");//��ʾUSB�����Ѿ�����
			else 
				Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,"USB DisConnected ");//��ʾUSB���γ���
			Divece_STA=bDeviceState;
		}
		
		tct++;
		
		if(tct==100)
		{
			tct=0;
						
			if(USB_STATUS_REG&0x10)
			{
				offline_cnt=0;//USB������,�����offline������
				bDeviceState=1;
				
				LED2=!LED2;//��ʾϵͳ������
			}
			else//û�еõ���ѯ 
			{
				offline_cnt++;  
				if(offline_cnt>10) bDeviceState=0;//5s��û�յ����߱��,����USB���γ���
			}
			
			USB_STATUS_REG=0;
		}
	};  
}




