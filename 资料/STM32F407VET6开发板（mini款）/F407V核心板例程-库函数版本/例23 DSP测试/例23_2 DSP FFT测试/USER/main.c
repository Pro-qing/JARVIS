#include "sys.h"
#include "delay.h"  
#include "usart.h"  
#include "led.h"
#include "key.h"
#include "GUI.h"
#include "Lcd_Driver.h"
#include "timer.h" 
#include "math.h" 
#include "arm_math.h"  

//STM32F407开发板 
//DSP FFT测试实验   -库函数版本



#define FFT_LENGTH		1024 		//FFT长度，默认是1024点FFT

float fft_inputbuf[FFT_LENGTH*2];	//FFT输入数组
float fft_outputbuf[FFT_LENGTH];	//FFT输出数组

u8 timeout;//定时器溢出次数


int main(void)
{ 
  arm_cfft_radix4_instance_f32 scfft;
 	u8 key,t=0;
	float time; 
	u8 buf[50]; 
	u16 i; 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	
	LED_Init();					//初始化LED
	KEY_Init();					//初始化按键
 	
	Lcd_Init();	 //1.8寸液晶屏--初始化配置
	Lcd_Clear(GRAY0);//清屏
	
	Gui_DrawFont_GBK16(0,16,RED,GRAY0," STM32F4xx 测试");
	Gui_DrawFont_GBK16(0,32,RED,GRAY0," DSP FFT TEST ");	
	Gui_DrawFont_GBK16(0,48,RED,GRAY0," 2018/5/5");
	
	TIM3_Int_Init(65535,84-1);	//1Mhz计数频率,最大计时65ms左右超出

	Gui_DrawFont_GBK16(0,64,RED,GRAY0," K1:Run FFT");//显示提示信息 
		
	arm_cfft_radix4_init_f32(&scfft,FFT_LENGTH,0,1);//初始化scfft结构体，设定FFT相关参数
	
 	while(1)
	{
		key=KEY_Scan(0);
		
		if(key==WKUP_PRES)
		{
			key=0;//清除键值
			
			for(i=0;i<FFT_LENGTH;i++)//生成信号序列
			{
				 fft_inputbuf[2*i]=100+
				                   10*arm_sin_f32(2*PI*i/FFT_LENGTH)+
								   30*arm_sin_f32(2*PI*i*4/FFT_LENGTH)+
				                   50*arm_cos_f32(2*PI*i*8/FFT_LENGTH);	//生成输入信号实部
				 fft_inputbuf[2*i+1]=0;//虚部全部为0
			}
			
			
			TIM_SetCounter(TIM3,0);//重设TIM3定时器的计数器值
			timeout=0;
			arm_cfft_radix4_f32(&scfft,fft_inputbuf);	//FFT计算（基4）
			time=TIM_GetCounter(TIM3)+(u32)timeout*65536; 			//计算所用时间
			
			sprintf((char*)buf,"FFT time:%0.3fms",time/1000);
			Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,buf);		  //显示运行时间	
			
			
			arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,FFT_LENGTH);	//把运算结果复数求模得幅值 
			printf("\r\n%d point FFT runtime:%0.3fms\r\n",FFT_LENGTH,time/1000);
			printf("FFT Result:\r\n");
			for(i=0;i<FFT_LENGTH;i++)
			{
				printf("fft_outputbuf[%d]:%f\r\n",i,fft_outputbuf[i]);
			}
		} else delay_ms(10);
		t++;
		if((t%10)==0)LED2=!LED2;		  
	}
}
 
