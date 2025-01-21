#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

#include "GUI.h"
#include "Lcd_Driver.h"

#include "key.h"  
#include "dma.h"


//STM32F407开发板 
//DMA 实验-库函数版本  



#define SEND_BUF_SIZE 8200	//发送数据长度,最好等于sizeof(TEXT_TO_SEND)+2的整数倍.

u8 SendBuff[SEND_BUF_SIZE];	//发送数据缓冲区
const u8 TEXT_TO_SEND[]={"Mcudev STM32F4 DMA 串口实验"};	 

 u8 tbuf[40];	//显示缓冲区
  
int main(void)
{ 
	u16 i;
	u8 t=0;
	u8 j,mask=0;
	float pro=0;//进度
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);     //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	LED_Init();					//初始化LED 
 	
	Lcd_Init();	 //1.8寸液晶屏--初始化配置
	Lcd_Clear(GRAY0);//清屏
	
	
	KEY_Init(); 				//按键初始化 
	
 	MYDMA_Config(DMA2_Stream7,DMA_Channel_4,(u32)&USART1->DR,(u32)SendBuff,SEND_BUF_SIZE);//DMA2,STEAM7,CH4,外设为串口1,存储器为SendBuff,长度为:SEND_BUF_SIZE.
	
	Gui_DrawFont_GBK16(0,16,RED,GRAY0," STM32F4xx 测试");
	Gui_DrawFont_GBK16(0,32,RED,GRAY0,"  DMA TEST");	
	Gui_DrawFont_GBK16(0,48,RED,GRAY0,"  2018/5/5");
  Gui_DrawFont_GBK16(0,64,RED,GRAY0,"  K1::Start");	 
	
	  	 
 //显示提示信息	
	j=sizeof(TEXT_TO_SEND);	   
	for(i=0;i<SEND_BUF_SIZE;i++)//填充ASCII字符集数据
    {
		if(t>=j)//加入换行符
		{
			if(mask)
			{
				SendBuff[i]=0x0a;
				t=0;
			}else 
			{
				SendBuff[i]=0x0d;
				mask++;
			}	
		}else//复制TEXT_TO_SEND语句
		{
			mask=0;
			SendBuff[i]=TEXT_TO_SEND[t];
			t++;
		}   	   
    }		 
	
	i=0;
	while(1)
	{
		t=KEY_Scan(0);
		if(t==WKUP_PRES)  //K1按下
		{
			t=0;//清除键值
			
			printf("\r\nDMA DATA:\r\n"); 
			
			Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"Start Transimit....");	 
			Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,"             %");//显示百分号   
			
						
      USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送     
			MYDMA_Enable(DMA2_Stream7,SEND_BUF_SIZE);     //开始一次DMA传输！	  
			
		    //等待DMA传输完成，此时我们来做另外一些事，点灯
		    //实际应用中，传输数据期间，可以执行另外的任务
		    while(1)
		    {
				if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)!=RESET)//等待DMA2_Steam7传输完成
				{ 
					DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);//清除DMA2_Steam7传输完成标志
					break; 
		        }
				pro=DMA_GetCurrDataCounter(DMA2_Stream7);//得到当前还剩余多少个数据
				pro=1-pro/SEND_BUF_SIZE;//得到百分比	  
				pro*=100;      			    //扩大100倍
						
				  
				sprintf((char*)tbuf,"  %4.2f",pro); //显示数据
				Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,tbuf);//显示百分号  	
						
		    }	
		  Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,"  100        %");//显示100%	 
			Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"Transimit Finished!   ");	 

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

