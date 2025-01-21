#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "GUI.h"
#include "Lcd_Driver.h"
#include "spi.h"
#include "w25qxx.h"
#include "key.h"  


//STM32F407开发板 
//SPI通信接口实验-库函数版本
 
 
//要写入到W25Q16的字符串数组
const u8 TEXT_Buffer[]={"mcudev STM32F4"};
#define SIZE sizeof(TEXT_Buffer)	 

u8 tbuf[40];//显示字符转换区


int main(void)
{ 
	u8 key;
	u16 i=0;
	u8 datatemp[SIZE];
	u32 FLASH_SIZE;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);     //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	LED_Init();					//初始化LED 
 	
	
	KEY_Init(); 				//按键初始化  
	W25QXX_Init();			//W25QXX初始化

	Lcd_Init();	 //1.8寸液晶屏--初始化配置
	Lcd_Clear(GRAY0);//清屏
	
	Gui_DrawFont_GBK16(0,16,RED,GRAY0," STM32F4xx 测试");
	Gui_DrawFont_GBK16(0,32,RED,GRAY0," SPI W25QXX TEST");	
	Gui_DrawFont_GBK16(0,48,RED,GRAY0," 2018/5/5");
	Gui_DrawFont_GBK16(0,64,RED,GRAY0," K1:Write&Read ");	//显示提示信息		
	
	
	while(W25QXX_ReadID()!=W25Q16)								//检测不到W25Q16
	{
		Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"W25Q16 Check Failed!");
		delay_ms(500);
		Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,"Please Check!      ");
		delay_ms(500);
		LED2=!LED2;		//DS0闪烁
	}
	Gui_DrawFont_GBK16(0,80,RED,GRAY0," W25Q16 Ready!"); 
	FLASH_SIZE=2*1024*1024;	//FLASH 大小为2字节
   
	while(1)
	{
		key=KEY_Scan(0);

		if(key==WKUP_PRES)//KEY1按下,写入W25Q16
		{
			 
 			Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,"Start Write W25Q16....");
			Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,"                     ");					//显示读到的字符串
			
			W25QXX_Write((u8*)TEXT_Buffer,FLASH_SIZE-100,SIZE);		//从倒数第100个地址处开始,写入SIZE长度的数据
			Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,"W25Q16 Write Finished!");	//提示传送完成
			
			Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,"Start Read W25Q16.... ");
			W25QXX_Read(datatemp,FLASH_SIZE-100,SIZE);					//从倒数第100个地址处开始,读出SIZE个字节
			Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,"The Data Readed Is:   ");	//提示传送完成
			sprintf((char*)tbuf,&datatemp[0]); 
			Gui_DrawFont_GBK16(0,112,BLUE,GRAY0,tbuf);					//显示读到的字符串
		}
		
		i++;
		delay_ms(10);
		if(i==20)
		{
			LED2=!LED2;//提示系统正在运行	
			i=0;
		}		   
	}       
}

