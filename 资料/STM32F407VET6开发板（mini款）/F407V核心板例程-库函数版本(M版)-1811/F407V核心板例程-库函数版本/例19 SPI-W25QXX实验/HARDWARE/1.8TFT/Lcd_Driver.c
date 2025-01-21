#include "sys.h"
#include "Lcd_Driver.h"
#include "LCD_Config.h"
#include "delay.h"

//嵌入式开发网
//淘宝网站：http://mcudev.taobao.com


//本测试程序使用的是模拟SPI接口驱动
//可自由更改接口IO配置，使用任意最少4 IO即可完成本款液晶驱动显示
/******************************************************************************
接口定义在Lcd_Driver.h内定义，请根据接线修改并修改相应IO初始化LCD_GPIO_Init()


#define LCD_BLK         //PB1    	  //背光控制
#define LCD_RS         	//PC5       //命令数据切换
#define LCD_CS        	//PB12      //片选
#define LCD_SDO/RST     //PB14      //复位
#define LCD_SCL        	//PB13      //时钟线
#define LCD_SDA        	//PB15      //数据线


*******************************************************************************/


//液晶IO初始化配置
void LCD_GPIO_Init(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;
	      
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC ,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_1 |GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_SetBits(GPIOB,GPIO_Pin_14);//MISO引脚输出为高
			
	LCD_BLK_SET;//开启背光
	LCD_BLK_CLR;//关闭背光
	LCD_BLK_SET;//开启背光
      
}


/****************************************************************************
* 名    称：void  SPIv_WriteData(u8 Data)
* 功    能：STM32_模拟SPI写一个字节数据底层函数
* 入口参数：Data
* 出口参数：无
* 说    明：STM32_模拟SPI读写一个字节数据底层函数
****************************************************************************/
void  SPI_WriteData(u8 Data)
{
	unsigned char i=0;
	for(i=8;i>0;i--)
	{
		if(Data&0x80)	
	  LCD_SDA_SET; //输出数据
      else LCD_SDA_CLR;
	   
      LCD_SCL_CLR;       
      LCD_SCL_SET;
      Data<<=1; 
	}
}


/****************************************************************************
* 名    称：void Lcd_WriteIndex(u8 idx)
* 功    能：写 ST7735S 控制器寄存器地址
* 入口参数：idx   寄存器地址
* 出口参数：无
* 说    明：调用前需先选中控制器，内部函数
* 调用方法：Lcd_WriteIndex(0x00);
****************************************************************************/
void Lcd_WriteIndex(u8 Index)
{
   
   //SPI 写命令时序开始
   LCD_CS_CLR;
   LCD_RS_CLR;  //写入命令
   SPI_WriteData(Index);
   LCD_CS_SET;
}

/****************************************************************************
* 名    称：void Lcd_WriteData(u8 dat)
* 功    能：写 ST7735S 寄存器数据
* 入口参数：dat     寄存器数据
* 出口参数：无
* 说    明：向控制器指定地址写入数据，调用前需先写寄存器地址，内部函数
* 调用方法：Lcd_WriteData(0x30)
****************************************************************************/
void Lcd_WriteData(u8 Data)
{
   
   LCD_CS_CLR;
   LCD_RS_SET;   //写入数据
   SPI_WriteData(Data);
   LCD_CS_SET; 
}


//写寄存器函数
void Lcd_WriteReg(u8 Index,u8 Data)
{
	Lcd_WriteIndex(Index);
  Lcd_WriteData(Data);
}


//向液晶屏写一个16位数据
void LCD_WriteData_16Bit(uint16_t Data)
{
   LCD_CS_CLR;
   LCD_RS_SET;
	 SPI_WriteData(Data>>8); 	//写入高8位数据
	 SPI_WriteData(Data); 			//写入低8位数据
   LCD_CS_SET; 
}

//液晶屏复位函数

void Lcd_Reset(void)
{
	  LCD_RST_CLR;
	  delay_ms(100);
	  LCD_RST_SET;
	  delay_ms(100);
}


//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

//设置LCD参数
//方便进行横竖屏模式切换
void LCD_SetParam(void)
{ 	
	lcddev.wramcmd=0x2C;
#if USE_HORIZONTAL==1	//使用横屏	  
	lcddev.dir=1;//横屏
	lcddev.width=160;
	lcddev.height=128;
	lcddev.setxcmd=0x2A;
	lcddev.setycmd=0x2B;			
	Lcd_WriteReg(0x36,0xA0);

#else//竖屏
	lcddev.dir=0;//竖屏				 	 		
	lcddev.width=128;
	lcddev.height=160;
	lcddev.setxcmd=0x2A;
	lcddev.setycmd=0x2B;	
	Lcd_WriteReg(0x36,0xC0);
#endif
}


/*************************************************
函数名：LCD_Set_Region
功能：设置lcd显示区域，在此区域写点数据自动换行
入口参数：xy起点和终点,Y_IncMode表示先自增y再自增x
返回值：无
*************************************************/
void Lcd_SetRegion(u16 x_start,u16 y_start,u16 x_end,u16 y_end)
{
#if USE_HORIZONTAL==1	//使用横屏
	
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
	
	Lcd_WriteIndex(0x2c); //开始写入GRAM

}

/*************************************************
函数名：LCD_Set_XY
功能：设置lcd显示起始点
入口参数：xy坐标
返回值：无
*************************************************/
void Lcd_SetXY(u16 x,u16 y)
{
  	//Lcd_SetRegion(x,y,x,y);
 #if USE_HORIZONTAL==1	//使用横屏	

	  Lcd_SetRegion(x+1,y+2,x+1,y+2);
	#else		
	  Lcd_SetRegion(x+2,y+1,x+2,y+1);
	#endif
}

	
/*************************************************
函数名：LCD_DrawPoint
功能：画一个点
入口参数：无
返回值：无
*************************************************/
void Gui_DrawPoint(u16 x,u16 y,u16 Data)
{
	//Lcd_SetRegion(x,y,x+1,y+1);
 #if USE_HORIZONTAL==1	//使用横屏	
	
  Lcd_SetRegion(x+1,y+2,x+2,y+3);
	
	#else	
	
	Lcd_SetRegion(x+2,y+1,x+3,y+2);

	#endif
	Lcd_WriteData(Data>>8);
	Lcd_WriteData(Data);

}    

/*****************************************
 函数功能：读TFT某一点的颜色                          
 出口参数：color  点颜色值                                 
******************************************/
//unsigned int Lcd_ReadPoint(u16 x,u16 y)
//{
//  unsigned int Data;
//  Lcd_SetXY(x,y);
//  Lcd_WriteData(Data);
//  return Data;
//}
/*************************************************
函数名：Lcd_Clear
功能：全屏清屏函数
入口参数：填充颜色COLOR
返回值：无
*************************************************/
void Lcd_Clear(u16 Color)               
{	
   unsigned int i,m;

#if USE_HORIZONTAL==1	//使用横屏	

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

	
	  LCD_GPIO_Init();//使用模拟SPI  初始化IO接口

	  Lcd_Reset();    //复位液晶屏


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

    LCD_SetParam();//设置LCD参数	 

}

