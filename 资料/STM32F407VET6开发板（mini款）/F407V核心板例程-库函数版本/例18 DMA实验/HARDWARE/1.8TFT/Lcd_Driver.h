#include "sys.h"


//嵌入式开发网
//淘宝网站：http://mcudev.taobao.com



//LCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	 wramcmd;		//开始写gram指令
	u16  setxcmd;		//设置x坐标指令
	u16  setycmd;		//设置y坐标指令	 
}_lcd_dev; 	

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数


#define RED  	  0xf800
#define GREEN	  0x07e0
#define BLUE 	  0x001f
#define WHITE	  0xffff
#define BLACK	  0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//灰色0 3165 00110 001011 00101
#define GRAY1   0x8410      	//灰色1      00000 000000 00000
#define GRAY2   0x4208      	//灰色2  1111111111011111

/////////////////////////////////////用户配置区///////////////////////////////////	 
//支持横竖屏快速定义切换，支持8/16位模式切换
#define USE_HORIZONTAL  	0	    //定义是否使用横屏 		0,不使用.  1,使用.
//使用模拟SPI作为测试



//液晶控制口置1操作语句宏定义
#define	LCD_SDA_SET  	GPIO_SetBits(GPIOB,GPIO_Pin_15)
#define	LCD_SCL_SET  	GPIO_SetBits(GPIOB,GPIO_Pin_13) 

#define	LCD_CS_SET  	GPIO_SetBits(GPIOB,GPIO_Pin_12)   
#define LCD_RST_SET   GPIO_SetBits(GPIOB,GPIO_Pin_14);//RST引脚输出为高

#define	LCD_RS_SET  	GPIO_SetBits(GPIOC,GPIO_Pin_5)   
#define	LCD_BLK_SET  	GPIO_SetBits(GPIOB,GPIO_Pin_1)   

//液晶控制口置0操作语句宏定义

#define	LCD_SDA_CLR  	GPIO_ResetBits(GPIOB,GPIO_Pin_15)    
#define	LCD_SCL_CLR  	GPIO_ResetBits(GPIOB,GPIO_Pin_13)  

#define	LCD_CS_CLR  	GPIO_ResetBits(GPIOB,GPIO_Pin_12)  
#define LCD_RST_CLR   GPIO_ResetBits(GPIOB,GPIO_Pin_14);//RST引脚输出为低

#define	LCD_RS_CLR  	GPIO_ResetBits(GPIOC,GPIO_Pin_5) 
#define	LCD_BLK_CLR  	GPIO_ResetBits(GPIOB,GPIO_Pin_1) 



void LCD_GPIO_Init(void);//初始化IO口
void  SPI_WriteData(u8 Data);//STM32_模拟SPI写一个字节数据底层函数
void LCD_WriteData_16Bit(uint16_t Data);//向液晶屏写一个16位数据

void Lcd_WriteIndex(u8 Index);//写控制器寄存器地址
void Lcd_WriteData(u8 Data);//  写寄存器数据
void Lcd_WriteReg(u8 Index,u8 Data);////写寄存器函数

u16 Lcd_ReadReg(u8 LCD_Reg);

void Lcd_Reset(void);
void Lcd_Init(void);
void Lcd_Clear(u16 Color);
void Lcd_SetXY(u16 x,u16 y);
void Gui_DrawPoint(u16 x,u16 y,u16 Data);
//unsigned int Lcd_ReadPoint(u16 x,u16 y);

