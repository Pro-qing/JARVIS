#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

#include "GUI.h"
#include "Lcd_Driver.h"

#include "key.h"  
#include "dma.h"


//STM32F407������ 
//DMA ʵ��-�⺯���汾  



#define SEND_BUF_SIZE 8200	//�������ݳ���,��õ���sizeof(TEXT_TO_SEND)+2��������.

u8 SendBuff[SEND_BUF_SIZE];	//�������ݻ�����
const u8 TEXT_TO_SEND[]={"Mcudev STM32F4 DMA ����ʵ��"};	 

 u8 tbuf[40];	//��ʾ������
  
int main(void)
{ 
	u16 i;
	u8 t=0;
	u8 j,mask=0;
	float pro=0;//����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);     //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED 
 	
	Lcd_Init();	 //1.8��Һ����--��ʼ������
	Lcd_Clear(GRAY0);//����
	
	
	KEY_Init(); 				//������ʼ�� 
	
 	MYDMA_Config(DMA2_Stream7,DMA_Channel_4,(u32)&USART1->DR,(u32)SendBuff,SEND_BUF_SIZE);//DMA2,STEAM7,CH4,����Ϊ����1,�洢��ΪSendBuff,����Ϊ:SEND_BUF_SIZE.
	
	Gui_DrawFont_GBK16(0,16,RED,GRAY0," STM32F4xx ����");
	Gui_DrawFont_GBK16(0,32,RED,GRAY0,"  DMA TEST");	
	Gui_DrawFont_GBK16(0,48,RED,GRAY0,"  2018/5/5");
  Gui_DrawFont_GBK16(0,64,RED,GRAY0,"  K1::Start");	 
	
	  	 
 //��ʾ��ʾ��Ϣ	
	j=sizeof(TEXT_TO_SEND);	   
	for(i=0;i<SEND_BUF_SIZE;i++)//���ASCII�ַ�������
    {
		if(t>=j)//���뻻�з�
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
		}else//����TEXT_TO_SEND���
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
		if(t==WKUP_PRES)  //K1����
		{
			t=0;//�����ֵ
			
			printf("\r\nDMA DATA:\r\n"); 
			
			Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"Start Transimit....");	 
			Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,"             %");//��ʾ�ٷֺ�   
			
						
      USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���1��DMA����     
			MYDMA_Enable(DMA2_Stream7,SEND_BUF_SIZE);     //��ʼһ��DMA���䣡	  
			
		    //�ȴ�DMA������ɣ���ʱ������������һЩ�£����
		    //ʵ��Ӧ���У����������ڼ䣬����ִ�����������
		    while(1)
		    {
				if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)!=RESET)//�ȴ�DMA2_Steam7�������
				{ 
					DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);//���DMA2_Steam7������ɱ�־
					break; 
		        }
				pro=DMA_GetCurrDataCounter(DMA2_Stream7);//�õ���ǰ��ʣ����ٸ�����
				pro=1-pro/SEND_BUF_SIZE;//�õ��ٷֱ�	  
				pro*=100;      			    //����100��
						
				  
				sprintf((char*)tbuf,"  %4.2f",pro); //��ʾ����
				Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,tbuf);//��ʾ�ٷֺ�  	
						
		    }	
		  Gui_DrawFont_GBK16(0,96,BLUE,GRAY0,"  100        %");//��ʾ100%	 
			Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"Transimit Finished!   ");	 

		}
		i++;
		delay_ms(10);
		if(i==20)
		{
			LED2=!LED2;//��ʾϵͳ��������	
			i=0;
		}		   
	}		    
}

