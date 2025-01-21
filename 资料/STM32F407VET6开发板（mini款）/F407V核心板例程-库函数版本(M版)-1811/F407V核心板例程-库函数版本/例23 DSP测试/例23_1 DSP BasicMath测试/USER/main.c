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

//STM32F407������ 
//DSP BasicMath����ʵ�� -�⺯���汾



#define	DELTA	0.000001f		//���ֵ
 
//sin cos����
//angle:��ʼ�Ƕ�
//times:�������
//mode:0,��ʹ��DSP��;1,ʹ��DSP��
//����ֵ��0,�ɹ�;0XFF,����
u8 sin_cos_test(float angle,u32 times,u8 mode)
{
	float sinx,cosx;
	float result;
	u32 i=0;
	if(mode==0)
	{
		for(i=0;i<times;i++)
		{
			cosx=cosf(angle);			//��ʹ��DSP�Ż���sin��cos����
			sinx=sinf(angle);
			result=sinx*sinx+cosx*cosx; //������Ӧ�õ���1  
			result=fabsf(result-1.0f);	//�Ա���1�Ĳ�ֵ
			if(result>DELTA)return 0XFF;//�ж�ʧ�� 
			angle+=0.001f;				//�Ƕ�����
		}
	}
	else
	{
		for(i=0;i<times;i++)
		{
			cosx=arm_cos_f32(angle);	//ʹ��DSP�Ż���sin��cos����
			sinx=arm_sin_f32(angle);
			result=sinx*sinx+cosx*cosx; //������Ӧ�õ���1  
			result=fabsf(result-1.0f);	//�Ա���1�Ĳ�ֵ
			if(result>DELTA)return 0XFF;//�ж�ʧ�� 
			angle+=0.001f;				//�Ƕ�����
		}
	}
	return 0;//�������
} 

u8 timeout;//��ʱ���������
 
int main(void)
{ 
	float time; 
	u8 buf[50];
	u8 res;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	
	LED_Init();					//��ʼ��LED
	KEY_Init();					//��ʼ������
 	
 	TIM3_Int_Init(65535,8400-1);//10Khz����Ƶ��,����ʱ6.5�볬��
	
	Lcd_Init();	 //1.8��Һ����--��ʼ������
	Lcd_Clear(GRAY0);//����
	
	Gui_DrawFont_GBK16(0,16,RED,GRAY0," STM32F4xx ����");
	Gui_DrawFont_GBK16(0,32,RED,GRAY0," DSP BasicMath ");	
	Gui_DrawFont_GBK16(0,48,RED,GRAY0," 2018/5/5");
	

	
	while(1)
	{
		
		//��ʹ��DSP�Ż�
		TIM_SetCounter(TIM3,0);//����TIM3��ʱ���ļ�����ֵ
		timeout=0;
		res=sin_cos_test(PI/6,200000,0);
		time=TIM_GetCounter(TIM3)+(u32)timeout*65536;
		sprintf((char*)buf," NoDSP:%0.1fms",time/10);//�����ݸ�ʽ������ʾ������
		
		if(res==0)

	    Gui_DrawFont_GBK16(0,64,BLUE,GRAY0,buf);		  //��ʾ����ʱ��	
		else 
			Gui_DrawFont_GBK16(0,64,BLUE,GRAY0," error��               ");	//��ʾ��ǰ�������	
		
		//ʹ��DSP�Ż�		
		TIM_SetCounter(TIM3,0);//����TIM3��ʱ���ļ�����ֵ
		
		timeout=0;
		res=sin_cos_test(PI/6,200000,1);
		time=TIM_GetCounter(TIM3)+(u32)timeout*65536;
		sprintf((char*)buf," UseDSP:%0.1fms",time/10);//�����ݸ�ʽ������ʾ������
		
		if(res==0)
			 Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,buf);		  //��ʾ����ʱ��	
		else 
			Gui_DrawFont_GBK16(0,80,BLUE,GRAY0," error��               ");	//��ʾ��ǰ�������	
		
		
		LED2=!LED2;
	}
}
 
