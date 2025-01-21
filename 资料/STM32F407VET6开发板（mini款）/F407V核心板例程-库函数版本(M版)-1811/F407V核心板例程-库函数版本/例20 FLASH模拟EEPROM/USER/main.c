#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "GUI.h"
#include "Lcd_Driver.h"
#include "stmflash.h" 
#include "key.h"  
//STM32F407开发板 
//FLASH模拟EEPROM 实验 -库函数版本


//要写入到STM32 FLASH的字符串数组
const u8 TEXT_Buffer[]={"STM32 FLASH TEST"};
#define TEXT_LENTH sizeof(TEXT_Buffer)	 		  	//数组长度	
#define SIZE TEXT_LENTH/4+((TEXT_LENTH%4)?1:0)

#define FLASH_SAVE_ADDR  0X0800C004 	//设置FLASH 保存地址(必须为偶数，且所在扇区,要大于本代码所占用到的扇区.
										//否则,写操作的时候,可能会导致擦除整个扇区,从而引起部分程序丢失.引起死机.

u8 tbuf[40];//显示缓冲区

int main(void)
{ 
	u8 key=0;
	u16 i=0;
	u8 datatemp[SIZE];	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	
	LED_Init();					//初始化LED 
 	
 	KEY_Init();					//按键初始化 

	Lcd_Init();	 //1.8寸液晶屏--初始化配置
	Lcd_Clear(GRAY0);//清屏
	
	Gui_DrawFont_GBK16(0,16,RED,GRAY0," STM32F4xx 测试");
	Gui_DrawFont_GBK16(0,32,RED,GRAY0," FLASH-EEPROM ");	
	Gui_DrawFont_GBK16(0,48,RED,GRAY0," 2018/5/5");
	Gui_DrawFont_GBK16(0,64,RED,GRAY0," K1:Write&Read ");	//显示提示信息	
	
	while(1)
	{
		key=KEY_Scan(0);
		if(key==WKUP_PRES)	//K1按下,写入STM32 FLASH
		{
			key=0;//清键值
 			Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"Start Write FLASH....");
			Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,"                     ");
			STMFLASH_Write(FLASH_SAVE_ADDR,(u32*)TEXT_Buffer,SIZE);
			Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"FLASH Write Finished!");//提示传送完成
			
			Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"Start Read FLASH.... ");
			STMFLASH_Read(FLASH_SAVE_ADDR,(u32*)datatemp,SIZE);
			Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"The Data Readed Is:  ");//提示传送完成
			
			sprintf((char*)tbuf,&datatemp[0]); 
			Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,tbuf);					//显示读到的字符串
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

