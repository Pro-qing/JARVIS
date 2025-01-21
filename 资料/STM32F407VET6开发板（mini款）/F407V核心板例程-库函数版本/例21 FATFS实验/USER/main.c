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



//STM32F407开发板 
//FATFS 实验 -库函数版本

 u8 tbuf[40];	//显示缓冲区

int main(void)
{        
 	u32 total,free;
	u8 t=0;	
	u8 res=0;	

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	LED_Init();					//初始化LED 
	 
 	KEY_Init();					//按键初始化 
	W25QXX_Init();				//初始化W25Q16
	my_mem_init(SRAMIN);		//初始化内部内存池 
	my_mem_init(SRAMCCM);		//初始化CCM内存池
	
	Lcd_Init();	 //1.8寸液晶屏--初始化配置
	Lcd_Clear(GRAY0);//清屏
	
	Gui_DrawFont_GBK16(0,16,RED,GRAY0," STM32F4xx 测试");
	Gui_DrawFont_GBK16(0,32,RED,GRAY0," FATFS TEST");	
	Gui_DrawFont_GBK16(0,48,RED,GRAY0," 2018/5/5");
	
	
	   
 	while(SD_Init())//检测不到SD卡
	{
		Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"SD Card Error!");
		delay_ms(500);					
		Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,"Please Check! ");
		delay_ms(500);
		LED2=!LED2;//DS0闪烁
	}
	
 	exfuns_init();							//为fatfs相关变量申请内存				 
  f_mount(fs[0],"0:",1); 					//挂载SD卡 
 	res=f_mount(fs[1],"1:",1); 				//挂载FLASH.	
	
	
	if(res==0X0D)//FLASH磁盘,FAT文件系统错误,重新格式化FLASH
	{
		Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"Flash Disk Formatting...");	//格式化FLASH
		res=f_mkfs("1:",1,4096);//格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇
		if(res==0)
		{
			f_setlabel((const TCHAR *)"1:mcudev");	//设置Flash磁盘的名字为：mcudev
			Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"Flash Disk Format Finish");	//格式化完成
		}
    else Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"Flash Disk Format Error ");	//格式化失败
		delay_ms(1000);
	}	

	
	Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"                    ");;	//清除显示	
				  
	while(exf_getfree("0",&total,&free))	//得到SD卡的总容量和剩余容量
	{
		Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"SD Card Fatfs Error!");
		delay_ms(200);
		Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"                    ");;	//清除显示			  
		delay_ms(200);
		LED2=!LED2;//DS0闪烁
	}													  			    
 	
	Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"FATFS OK!");	

	sprintf((char*)tbuf,"SD Total:%d MB",total>>10); //显示SD卡总容量 MB
	Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,tbuf);
	sprintf((char*)tbuf,"SD Free:%d MB",free>>10); //显示SD卡剩余容量 MB		
	Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,tbuf);	
	
//	LCD_ShowString(30,170,200,16,16,"SD Total Size:     MB");	 
//	LCD_ShowString(30,190,200,16,16,"SD  Free Size:     MB"); 	    
// 	LCD_ShowNum(30+8*14,170,total>>10,5,16);				//显示SD卡总容量 MB
// 	LCD_ShowNum(30+8*14,190,free>>10,5,16);					//显示SD卡剩余容量 MB			

	
	while(1)
	{
		t++; 
		delay_ms(200);		 			   
		LED2=!LED2;
	} 
}





