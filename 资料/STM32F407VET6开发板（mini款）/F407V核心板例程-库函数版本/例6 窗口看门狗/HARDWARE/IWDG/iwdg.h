#ifndef _IWDG_H
#define _IWDG_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 

//STM32F407开发板
//独立看门狗 驱动代码	   
			
//********************************************************************************

////////////////////////////////////////////////////////////////////////////////// 	

void IWDG_Init(u8 prer,u16 rlr);//IWDG初始化
void IWDG_Feed(void);  //喂狗函数
#endif
