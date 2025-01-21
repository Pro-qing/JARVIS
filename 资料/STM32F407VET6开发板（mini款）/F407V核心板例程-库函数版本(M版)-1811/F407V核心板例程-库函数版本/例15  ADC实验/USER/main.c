#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "GUI.h"
#include "Lcd_Driver.h"
#include "adc.h"


//STM32F407开发板 
//ADC模数转换实验-库函数版本  

 	u16 adcx;
	float temp;
  u8 tbuf[40];	//显示缓冲区

int main(void)
{ 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	LED_Init();					//初始化LED 
	
  Lcd_Init();	 //1.8寸液晶屏--初始化配置
	Lcd_Clear(GRAY0);//清屏
	
	Adc_Init();         //初始化ADC
	
	Gui_DrawFont_GBK16(0,16,RED,GRAY0," STM32F4xx 测试");
	Gui_DrawFont_GBK16(0,32,RED,GRAY0,"  ADC TEST");	
	Gui_DrawFont_GBK16(0,48,RED,GRAY0,"  2018/5/5");	

	
	Gui_DrawFont_GBK16(4,64,BLUE,GRAY0,"ADC1_CH5_VAL:");	      
	Gui_DrawFont_GBK16(4,80,BLUE,GRAY0,"ADC1_CH5_VOL:0.000V");	//先在固定位置显示小数点      
	while(1)
	{
		
		adcx=Get_Adc_Average(ADC_Channel_5,20);//获取通道5的转换值，20次取平均
		
				
		sprintf((char*)tbuf,"ADC:0x%x",adcx); //显示ADCC采样后的原始值
		Gui_DrawFont_GBK16(4,64,BLUE,GRAY0,"                 ");	//清除上次的显示
		Gui_DrawFont_GBK16(4,64,BLUE,GRAY0,tbuf);	//液晶屏显示随机数
		
		temp=(float)adcx*(3.3/4096);          //获取计算后的带小数的实际电压值，比如3.1111

		sprintf((char*)tbuf,"ADC: %5.3f",temp); //显示浮点数
		Gui_DrawFont_GBK16(4,80,BLUE,GRAY0,"                 ");	//清除上次的显示
		Gui_DrawFont_GBK16(4,80,BLUE,GRAY0,tbuf);	//液晶屏显示随机数
		
		LED2=!LED2;
		delay_ms(250);	
		
	}
}



