#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "GUI.h"
#include "Lcd_Driver.h"
#include "key.h"  
#include "malloc.h" 
#include "sdio_sdcard.h"    
//STM32F407开发板 
//SD卡 实验 -库函数版本



//通过串口打印SD卡相关信息
void show_sdcard_info(void)
{
	switch(SDCardInfo.CardType)
	{
		case SDIO_STD_CAPACITY_SD_CARD_V1_1:printf("Card Type:SDSC V1.1\r\n");break;
		case SDIO_STD_CAPACITY_SD_CARD_V2_0:printf("Card Type:SDSC V2.0\r\n");break;
		case SDIO_HIGH_CAPACITY_SD_CARD:printf("Card Type:SDHC V2.0\r\n");break;
		case SDIO_MULTIMEDIA_CARD:printf("Card Type:MMC Card\r\n");break;
	}	
  	printf("Card ManufacturerID:%d\r\n",SDCardInfo.SD_cid.ManufacturerID);	//制造商ID
 	printf("Card RCA:%d\r\n",SDCardInfo.RCA);								//卡相对地址
	printf("Card Capacity:%d MB\r\n",(u32)(SDCardInfo.CardCapacity>>20));	//显示容量
 	printf("Card BlockSize:%d\r\n\r\n",SDCardInfo.CardBlockSize);			//显示块大小
}


 u8 tbuf[40];	//显示缓冲区


int main(void)
{        
	u8 key;		 
	u32 sd_size;
	u8 t=0;	
	u8 *buf;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	LED_Init();					//初始化LED  
 	
 	KEY_Init();					//按键初始化  
	
	my_mem_init(SRAMIN);		//初始化内部内存池 
	my_mem_init(SRAMCCM);		//初始化CCM内存池
	
	Lcd_Init();	 //1.8寸液晶屏--初始化配置
	Lcd_Clear(GRAY0);//清屏
	
	Gui_DrawFont_GBK16(0,16,RED,GRAY0," STM32F4xx 测试");
	Gui_DrawFont_GBK16(0,32,RED,GRAY0," SD Card TEST");	
	Gui_DrawFont_GBK16(0,48,RED,GRAY0," 2018/5/5");
	Gui_DrawFont_GBK16(0,64,RED,GRAY0," K1:Write&Read ");	//显示提示信息	
  
   
 	while(SD_Init())//检测不到SD卡
	{
		Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"SD Card Error!");
		delay_ms(500);					
		Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,"Please Check! ");
		delay_ms(500);
		LED2=!LED2;//DS0闪烁
	}
	show_sdcard_info();	//打印SD卡相关信息
 	
	//检测SD卡成功 											    
	Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"SD Card OK    ");
	
	
	sprintf((char*)tbuf,"SD Size:%d MB",SDCardInfo.CardCapacity>>20); //显示SD卡容量
	Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,tbuf);
	
	
	
	while(1)
	{
		key=KEY_Scan(0);
		if(key==WKUP_PRES)//K1按下了
		{
			key=0;//清除键值
			
			buf=mymalloc(0,512);		//申请内存
			
			if(SD_ReadDisk(buf,0,1)==0)	//读取0扇区的内容
			{	
				Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,"USART1 Sending Data...");
				printf("SECTOR 0 DATA:\r\n");
				for(sd_size=0;sd_size<512;sd_size++)printf("%x ",buf[sd_size]);//打印0扇区数据    	   
				printf("\r\nDATA ENDED\r\n");
				Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,"USART1 Send Data Over!");
			}
			myfree(0,buf);//释放内存	   
		}   
		t++;
		delay_ms(10);
		if(t==20)
		{
			LED2=!LED2;
			t=0;
		}
	} }





