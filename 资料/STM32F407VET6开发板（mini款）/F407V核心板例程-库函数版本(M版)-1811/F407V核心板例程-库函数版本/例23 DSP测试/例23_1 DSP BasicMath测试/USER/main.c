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
//DSP BasicMath测试实验 -库函数版本



#define	DELTA	0.000001f		//误差值
 
//sin cos测试
//angle:起始角度
//times:运算次数
//mode:0,不使用DSP库;1,使用DSP库
//返回值：0,成功;0XFF,出错
u8 sin_cos_test(float angle,u32 times,u8 mode)
{
	float sinx,cosx;
	float result;
	u32 i=0;
	if(mode==0)
	{
		for(i=0;i<times;i++)
		{
			cosx=cosf(angle);			//不使用DSP优化的sin，cos函数
			sinx=sinf(angle);
			result=sinx*sinx+cosx*cosx; //计算结果应该等于1  
			result=fabsf(result-1.0f);	//对比与1的差值
			if(result>DELTA)return 0XFF;//判断失败 
			angle+=0.001f;				//角度自增
		}
	}
	else
	{
		for(i=0;i<times;i++)
		{
			cosx=arm_cos_f32(angle);	//使用DSP优化的sin，cos函数
			sinx=arm_sin_f32(angle);
			result=sinx*sinx+cosx*cosx; //计算结果应该等于1  
			result=fabsf(result-1.0f);	//对比与1的差值
			if(result>DELTA)return 0XFF;//判断失败 
			angle+=0.001f;				//角度自增
		}
	}
	return 0;//任务完成
} 

u8 timeout;//定时器溢出次数
 
int main(void)
{ 
	float time; 
	u8 buf[50];
	u8 res;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	
	LED_Init();					//初始化LED
	KEY_Init();					//初始化按键
 	
 	TIM3_Int_Init(65535,8400-1);//10Khz计数频率,最大计时6.5秒超出
	
	Lcd_Init();	 //1.8寸液晶屏--初始化配置
	Lcd_Clear(GRAY0);//清屏
	
	Gui_DrawFont_GBK16(0,16,RED,GRAY0," STM32F4xx 测试");
	Gui_DrawFont_GBK16(0,32,RED,GRAY0," DSP BasicMath ");	
	Gui_DrawFont_GBK16(0,48,RED,GRAY0," 2018/5/5");
	

	
	while(1)
	{
		
		//不使用DSP优化
		TIM_SetCounter(TIM3,0);//重设TIM3定时器的计数器值
		timeout=0;
		res=sin_cos_test(PI/6,200000,0);
		time=TIM_GetCounter(TIM3)+(u32)timeout*65536;
		sprintf((char*)buf," NoDSP:%0.1fms",time/10);//将数据格式化到显示缓冲区
		
		if(res==0)

	    Gui_DrawFont_GBK16(0,64,BLUE,GRAY0,buf);		  //显示运行时间	
		else 
			Gui_DrawFont_GBK16(0,64,BLUE,GRAY0," error！               ");	//显示当前运行情况	
		
		//使用DSP优化		
		TIM_SetCounter(TIM3,0);//重设TIM3定时器的计数器值
		
		timeout=0;
		res=sin_cos_test(PI/6,200000,1);
		time=TIM_GetCounter(TIM3)+(u32)timeout*65536;
		sprintf((char*)buf," UseDSP:%0.1fms",time/10);//将数据格式化到显示缓冲区
		
		if(res==0)
			 Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,buf);		  //显示运行时间	
		else 
			Gui_DrawFont_GBK16(0,80,BLUE,GRAY0," error！               ");	//显示当前运行情况	
		
		
		LED2=!LED2;
	}
}
 
