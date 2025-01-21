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

//探索者STM32F407开发板 
//USB读卡器 实验 -库函数版本

 

USB_OTG_CORE_HANDLE USB_OTG_dev;
extern vu8 USB_STATUS_REG;		//USB状态
extern vu8 bDeviceState;		//USB连接 情况

 u8 tbuf[40];	//显示缓冲区

int main(void)
{        
	u8 offline_cnt=0;
	u8 tct=0;
	u8 USB_STA;
	u8 Divece_STA;


	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	LED_Init();					//初始化LED  
 	
 	KEY_Init();					//按键初始化  
	W25QXX_Init();				//初始化W25Q16  
  
	
	Lcd_Init();	 //1.8寸液晶屏--初始化配置
	Lcd_Clear(GRAY0);//清屏
	
	Gui_DrawFont_GBK16(0,16,RED,GRAY0," STM32F4xx 测试");
	Gui_DrawFont_GBK16(0,32,RED,GRAY0," USB Card TEST");	
	Gui_DrawFont_GBK16(0,48,RED,GRAY0," 2018/5/5");
	




	if(SD_Init())
		Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"SD Card Error!");	//检测SD卡错误
	else //SD 卡正常
	{   															  
		
		sprintf((char*)tbuf,"SD Size:%d MB",SDCardInfo.CardCapacity>>20); //显示SD卡容量
	  Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,tbuf);
		
 	}
	
	if(W25QXX_ReadID()!=W25Q16)
		Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,"W25Q16 Error!");	//检测W25Q128错误
	else //SPI FLASH 正常
	{   														 
		Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,"W25Q16 Size:2MB");	
 		
	}  
 	Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,"USB Connecting...");//提示正在建立连接 	   
	
	USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_MSC_cb,&USR_cb);
	
	delay_ms(1800);	
	
	while(1)
	{	
		delay_ms(5);		
		
		if(USB_STA!=USB_STATUS_REG)//状态改变了 
		{	 						   
			Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,"                ");//清除显示			  	   
			if(USB_STATUS_REG&0x01)//正在写		  
			{
				LED2=0;
				Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,"USB Writing...");//提示USB正在写入数据	 
			}
			if(USB_STATUS_REG&0x02)//正在读
			{
				LED2=0;
				Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,"USB Reading...");//提示USB正在读出数据  		 
			}	 										  
			if(USB_STATUS_REG&0x04)
				Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,"USB Write Err ");//提示写入错误
			else 
				Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,"                ");//清除显示		  
			if(USB_STATUS_REG&0x08)
				Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,"USB Read  Err ");//提示读出错误
			else 
				Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,"                ");//清除显示	 
			USB_STA=USB_STATUS_REG;//记录最后的状态
		}
		
		if(Divece_STA!=bDeviceState) 
		{
			if(bDeviceState==1)
				Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,"USB Connected    ");//提示USB连接已经建立
			else 
				Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,"USB DisConnected ");//提示USB被拔出了
			Divece_STA=bDeviceState;
		}
		
		tct++;
		
		if(tct==100)
		{
			tct=0;
						
			if(USB_STATUS_REG&0x10)
			{
				offline_cnt=0;//USB连接了,则清除offline计数器
				bDeviceState=1;
				
				LED2=!LED2;//提示系统在运行
			}
			else//没有得到轮询 
			{
				offline_cnt++;  
				if(offline_cnt>10) bDeviceState=0;//5s内没收到在线标记,代表USB被拔出了
			}
			
			USB_STATUS_REG=0;
		}
	};  
}




