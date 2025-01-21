#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

#include "GUI.h"
#include "Lcd_Driver.h"

#include "adc.h"
#include "dac.h"
#include "key.h"


//STM32F407开发板 
//DAC实验-库函数版本


	u8 key;	
  u8 tbuf[40];	//显示缓冲区
	
int main(void)
{ 
  u16 adcx;
	float temp;
 	u8 t=0;	 
	u16 dacval=0;
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);      //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	
	LED_Init();					//初始化LED 
 
	Lcd_Init();	 //1.8寸液晶屏--初始化配置
	Lcd_Clear(GRAY0);//清屏
	
	Adc_Init(); 				//adc初始化
	KEY_Init(); 				//按键初始化
	Dac1_Init();		 		//DAC通道1初始化	
	
	
	Gui_DrawFont_GBK16(0,16,RED,GRAY0," STM32F4xx 测试");
	Gui_DrawFont_GBK16(0,32,RED,GRAY0,"  DAC TEST");	
	Gui_DrawFont_GBK16(0,48,RED,GRAY0,"  2018/5/5");
	Gui_DrawFont_GBK16(0,64,RED,GRAY0,"  K1::+  ");	 
	
	    	 
	Gui_DrawFont_GBK16(0,80, BLUE,GRAY0," DAC VAL:");	      
	Gui_DrawFont_GBK16(0,96, BLUE,GRAY0," DAC VOL:0.000V");	      
	Gui_DrawFont_GBK16(0,112,BLUE,GRAY0," ADC VOL:0.000V");
 	
  DAC_SetChannel1Data(DAC_Align_12b_R,dacval);//初始值为0	
	while(1)
	{
		t++;
		
		key=KEY_Scan(0);	//按键扫描
		
		if(key==WKUP_PRES)//按按键，增加DAC输出值
		{		
      key=0;//清除键值			
			if(dacval<4000)dacval+=200;
			DAC_SetChannel1Data(DAC_Align_12b_R, dacval);//设置DAC值
		}
	 
		
		if(t==10||key==WKUP_PRES) 	//K1按下了,或者定时时间到了
		{	  
 			adcx=DAC_GetDataOutputValue(DAC_Channel_1);//读取前面设置DAC的值
			
			
			sprintf((char*)tbuf," DAC VAL:0x%x",adcx); //显示ADCC采样后的原始值
		  Gui_DrawFont_GBK16(4,80,BLUE,GRAY0," DAC VAL:            ");	//清除上次的显示
		  Gui_DrawFont_GBK16(4,80,BLUE,GRAY0,tbuf);	//液晶屏显示随机数
			
			
			temp=(float)adcx*(3.3/4096);			         //得到DAC电压值
			
			sprintf((char*)tbuf," DAC VOL: %5.3f",temp); //显示浮点数
		  Gui_DrawFont_GBK16(4,96,BLUE,GRAY0," DAC VOL:            ");	//清除上次的显示
		  Gui_DrawFont_GBK16(4,96,BLUE,GRAY0,tbuf);	//液晶屏显示随机数
			
			
 			adcx=Get_Adc_Average(ADC_Channel_5,10);		//得到ADC转换值	  
			temp=(float)adcx*(3.3/4096);			        //得到ADC电压值
			
			sprintf((char*)tbuf," ADC VOL: %5.3f",temp); //显示浮点数
		  Gui_DrawFont_GBK16(4,112,BLUE,GRAY0," ADC VOL:            ");	//清除上次的显示
		  Gui_DrawFont_GBK16(4,112,BLUE,GRAY0,tbuf);	//液晶屏显示随机数			

			
			LED2=!LED2;	   
			t=0;
		}	    
		delay_ms(10);	 
	}	
}

