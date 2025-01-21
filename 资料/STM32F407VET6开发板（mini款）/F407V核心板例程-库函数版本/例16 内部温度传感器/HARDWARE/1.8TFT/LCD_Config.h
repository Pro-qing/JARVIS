
#include "sys.h"

//嵌入式开发网
//淘宝网站：http://mcudev.taobao.com


//==================================如何切换横竖屏显示=======================================//
//打开Lcd_Driver.h头文件，修改宏#define USE_HORIZONTAL 值为0使用竖屏模式.1,使用横屏模式
//===========================模拟SPI总线驱动==========================================//


//定义LCD的尺寸

#if USE_HORIZONTAL==1	//使用横屏
#define X_MAX_PIXEL	        160
#define Y_MAX_PIXEL	        128
#else
#define X_MAX_PIXEL	        128
#define Y_MAX_PIXEL	        160
#endif
