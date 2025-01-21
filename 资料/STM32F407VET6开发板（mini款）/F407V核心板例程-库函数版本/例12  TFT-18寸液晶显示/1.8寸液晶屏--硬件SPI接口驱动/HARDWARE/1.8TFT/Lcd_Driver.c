#include "sys.h"
#include "Lcd_Driver.h"
#include "LCD_Config.h"
#include "delay.h"

//Ƕ��ʽ������
//�Ա���վ��http://mcudev.taobao.com


//�����Գ���ʹ�õ���ģ��SPI�ӿ�����
//�����ɸ��Ľӿ�IO���ã�ʹ����������4 IO������ɱ���Һ��������ʾ
/******************************************************************************
�ӿڶ�����Lcd_Driver.h�ڶ��壬����ݽ����޸Ĳ��޸���ӦIO��ʼ��LCD_GPIO_Init()


#define LCD_BLK         //PB1    	  //�������
#define LCD_RS         	//PC5       //���������л�
#define LCD_CS        	//PB12      //Ƭѡ
#define LCD_SDO/RST     //PB14      //��λ
#define LCD_SCL        	//PB13      //ʱ����
#define LCD_SDA        	//PB15      //������


*******************************************************************************/


//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI2�ĳ�ʼ��
void SPI2_Init(void)
{	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��PBʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//ʹ��SPI2ʱ��
 
  //PB13--SCL ,PB15--SDI��ʼ������, 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15;//PB13,PB15���ù������	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2); //PB13����Ϊ SPI2
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2); //PB15����Ϊ SPI2
 
	//����ֻ���SPI�ڳ�ʼ��
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,ENABLE);//��λSPI2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,DISABLE);//ֹͣ��λSPI2

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:ȫ��˫��ģʽ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����

	SPI2_ReadWriteByte(0xff);//��������		 
}   

//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI2_ReadWriteByte(u8 TxData)
{		 			 
 
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������  
	
	SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ��byte  ����
	
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){} //�ȴ�������һ��byte  
 
	return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����	
   
}






//Һ��IO��ʼ������
void LCD_GPIO_Init(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;
	      
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC ,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_1  |GPIO_Pin_12  | GPIO_Pin_14 ;//| GPIO_Pin_13, |GPIO_Pin_15
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_SetBits(GPIOB,GPIO_Pin_14);//MISO�������Ϊ��
	
  SPI2_Init();//��ʼ��Ӳ��SPI2

	LCD_BLK_SET;//��������
	LCD_BLK_CLR;//�رձ���
	LCD_BLK_SET;//��������
      
}


/****************************************************************************
* ��    �ƣ�void  SPIv_WriteData(u8 Data)
* ��    �ܣ�STM32_ģ��SPIдһ���ֽ����ݵײ㺯��
* ��ڲ�����Data
* ���ڲ�������
* ˵    ����STM32_ģ��SPI��дһ���ֽ����ݵײ㺯��
****************************************************************************/
void  SPI_WriteData(u8 Data)
{
	SPI2_ReadWriteByte(Data);//��������
}


/****************************************************************************
* ��    �ƣ�void Lcd_WriteIndex(u8 idx)
* ��    �ܣ�д ST7735S �������Ĵ�����ַ
* ��ڲ�����idx   �Ĵ�����ַ
* ���ڲ�������
* ˵    ��������ǰ����ѡ�п��������ڲ�����
* ���÷�����Lcd_WriteIndex(0x00);
****************************************************************************/
void Lcd_WriteIndex(u8 Index)
{
   
   //SPI д����ʱ��ʼ
   LCD_CS_CLR;
   LCD_RS_CLR;  //д������
   SPI_WriteData(Index);
   LCD_CS_SET;
}

/****************************************************************************
* ��    �ƣ�void Lcd_WriteData(u8 dat)
* ��    �ܣ�д ST7735S �Ĵ�������
* ��ڲ�����dat     �Ĵ�������
* ���ڲ�������
* ˵    �����������ָ����ַд�����ݣ�����ǰ����д�Ĵ�����ַ���ڲ�����
* ���÷�����Lcd_WriteData(0x30)
****************************************************************************/
void Lcd_WriteData(u8 Data)
{
   
   LCD_CS_CLR;
   LCD_RS_SET;   //д������
   SPI_WriteData(Data);
   LCD_CS_SET; 
}


//д�Ĵ�������
void Lcd_WriteReg(u8 Index,u8 Data)
{
	Lcd_WriteIndex(Index);
  Lcd_WriteData(Data);
}


//��Һ����дһ��16λ����
void LCD_WriteData_16Bit(uint16_t Data)
{
   LCD_CS_CLR;
   LCD_RS_SET;
	 SPI_WriteData(Data>>8); 	//д���8λ����
	 SPI_WriteData(Data); 			//д���8λ����
   LCD_CS_SET; 
}

//Һ������λ����

void Lcd_Reset(void)
{
	  LCD_RST_CLR;
	  delay_ms(100);
	  LCD_RST_SET;
	  delay_ms(100);
}


//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;

//����LCD����
//������к�����ģʽ�л�
void LCD_SetParam(void)
{ 	
	lcddev.wramcmd=0x2C;
#if USE_HORIZONTAL==1	//ʹ�ú���	  
	lcddev.dir=1;//����
	lcddev.width=160;
	lcddev.height=128;
	lcddev.setxcmd=0x2A;
	lcddev.setycmd=0x2B;			
	Lcd_WriteReg(0x36,0xA0);

#else//����
	lcddev.dir=0;//����				 	 		
	lcddev.width=128;
	lcddev.height=160;
	lcddev.setxcmd=0x2A;
	lcddev.setycmd=0x2B;	
	Lcd_WriteReg(0x36,0xC0);
#endif
}


/*************************************************
��������LCD_Set_Region
���ܣ�����lcd��ʾ�����ڴ�����д�������Զ�����
��ڲ�����xy�����յ�,Y_IncMode��ʾ������y������x
����ֵ����
*************************************************/
void Lcd_SetRegion(u16 x_start,u16 y_start,u16 x_end,u16 y_end)
{
#if USE_HORIZONTAL==1	//ʹ�ú���
	
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_start);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_end);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_start);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_end);
	
	
#else	
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_start);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_end);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_start);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_end);
#endif
	
	Lcd_WriteIndex(0x2c); //��ʼд��GRAM

}

/*************************************************
��������LCD_Set_XY
���ܣ�����lcd��ʾ��ʼ��
��ڲ�����xy����
����ֵ����
*************************************************/
void Lcd_SetXY(u16 x,u16 y)
{
  	//Lcd_SetRegion(x,y,x,y);
 #if USE_HORIZONTAL==1	//ʹ�ú���	

	  Lcd_SetRegion(x+1,y+2,x+1,y+2);
	#else		
	  Lcd_SetRegion(x+2,y+1,x+2,y+1);
	#endif
}

	
/*************************************************
��������LCD_DrawPoint
���ܣ���һ����
��ڲ�������
����ֵ����
*************************************************/
void Gui_DrawPoint(u16 x,u16 y,u16 Data)
{
	//Lcd_SetRegion(x,y,x+1,y+1);
 #if USE_HORIZONTAL==1	//ʹ�ú���	
	
  Lcd_SetRegion(x+1,y+2,x+2,y+3);
	
	#else	
	
	Lcd_SetRegion(x+2,y+1,x+3,y+2);

	#endif
	Lcd_WriteData(Data>>8);
	Lcd_WriteData(Data);

}    

/*****************************************
 �������ܣ���TFTĳһ�����ɫ                          
 ���ڲ�����color  ����ɫֵ                                 
******************************************/
//unsigned int Lcd_ReadPoint(u16 x,u16 y)
//{
//  unsigned int Data;
//  Lcd_SetXY(x,y);
//  Lcd_WriteData(Data);
//  return Data;
//}
/*************************************************
��������Lcd_Clear
���ܣ�ȫ����������
��ڲ����������ɫCOLOR
����ֵ����
*************************************************/
void Lcd_Clear(u16 Color)               
{	
   unsigned int i,m;

#if USE_HORIZONTAL==1	//ʹ�ú���	

   //Lcd_SetRegion(2,1,X_MAX_PIXEL+1,Y_MAX_PIXEL);
	 Lcd_SetRegion(1,2,X_MAX_PIXEL,Y_MAX_PIXEL+1);
	
   Lcd_WriteIndex(0x2C);
	
 	for(i=0;i<Y_MAX_PIXEL;i++)
    for(m=0;m<X_MAX_PIXEL;m++)
    {	
	  	Lcd_WriteData(Color>>8);
			Lcd_WriteData(Color);
    } 
		
	
#else	
   Lcd_SetRegion(2,1,X_MAX_PIXEL+1,Y_MAX_PIXEL);
	
   Lcd_WriteIndex(0x2C);
	
 	for(i=0;i<Y_MAX_PIXEL;i++)
    for(m=0;m<X_MAX_PIXEL;m++)
    {	
	  	Lcd_WriteData(Color>>8);
			Lcd_WriteData(Color);
    } 
		
#endif	
		
}

void Lcd_Init(void)
{

	
	  LCD_GPIO_Init();//ʹ��ģ��SPI  ��ʼ��IO�ӿ�

	  Lcd_Reset();    //��λҺ����


		Lcd_WriteIndex(0x11);//Sleep exit 
		
		delay_ms (200);
			
		Lcd_WriteIndex(0xB1); 
		Lcd_WriteData(0x05); 
		Lcd_WriteData(0x3C); 
		Lcd_WriteData(0x3C); 
		Lcd_WriteIndex(0xB2); 
		Lcd_WriteData(0x05); 
		Lcd_WriteData(0x3C); 
		Lcd_WriteData(0x3C); 
		Lcd_WriteIndex(0xB3); 
		Lcd_WriteData(0x05); 
		Lcd_WriteData(0x3C); 
		Lcd_WriteData(0x3C); 
		Lcd_WriteData(0x05); 
		Lcd_WriteData(0x3C); 
		Lcd_WriteData(0x3C); 
		//------------------------------------End ST7735S Frame Rate---------------------------------// 
		Lcd_WriteIndex(0xB4); //Dot inversion 
		Lcd_WriteData(0x03); 
		//------------------------------------ST7735S Power Sequence---------------------------------// 
		Lcd_WriteIndex(0xC0); 
		Lcd_WriteData(0x28); 
		Lcd_WriteData(0x08); 
		Lcd_WriteData(0x04); 
		Lcd_WriteIndex(0xC1); 
		Lcd_WriteData(0XC0); 
		Lcd_WriteIndex(0xC2); 
		Lcd_WriteData(0x0D); 
		Lcd_WriteData(0x00); 
		Lcd_WriteIndex(0xC3); 
		Lcd_WriteData(0x8D); 
		Lcd_WriteData(0x2A); 
		Lcd_WriteIndex(0xC4); 
		Lcd_WriteData(0x8D); 
		Lcd_WriteData(0xEE); 
		//---------------------------------End ST7735S Power Sequence-------------------------------------// 
		Lcd_WriteIndex(0xC5); //VCOM 
		Lcd_WriteData(0x1A); 
		Lcd_WriteIndex(0x36); //MX, MY, RGB mode 
		Lcd_WriteData(0xC0); 
		//------------------------------------ST7735S Gamma Sequence---------------------------------// 
		Lcd_WriteIndex(0xE0); 
		Lcd_WriteData(0x04); 
		Lcd_WriteData(0x22); 
		Lcd_WriteData(0x07); 
		Lcd_WriteData(0x0A); 
		Lcd_WriteData(0x2E); 
		Lcd_WriteData(0x30); 
		Lcd_WriteData(0x25); 
		Lcd_WriteData(0x2A); 
		Lcd_WriteData(0x28); 
		Lcd_WriteData(0x26); 
		Lcd_WriteData(0x2E); 
		Lcd_WriteData(0x3A); 
		Lcd_WriteData(0x00); 
		Lcd_WriteData(0x01); 
		Lcd_WriteData(0x03); 
		Lcd_WriteData(0x13); 
		Lcd_WriteIndex(0xE1); 
		Lcd_WriteData(0x04); 
		Lcd_WriteData(0x16); 
		Lcd_WriteData(0x06); 
		Lcd_WriteData(0x0D); 
		Lcd_WriteData(0x2D); 
		Lcd_WriteData(0x26); 
		Lcd_WriteData(0x23); 
		Lcd_WriteData(0x27); 
		Lcd_WriteData(0x27); 
		Lcd_WriteData(0x25); 
		Lcd_WriteData(0x2D); 
		Lcd_WriteData(0x3B); 
		Lcd_WriteData(0x00); 
		Lcd_WriteData(0x01); 
		Lcd_WriteData(0x04); 
		Lcd_WriteData(0x13); 
		//------------------------------------End ST7735S Gamma Sequence-----------------------------// 
		Lcd_WriteIndex(0x3A); //65k mode 
		Lcd_WriteData(0x05); 
		Lcd_WriteIndex(0x29); //Display on 

    LCD_SetParam();//����LCD����	 

}

