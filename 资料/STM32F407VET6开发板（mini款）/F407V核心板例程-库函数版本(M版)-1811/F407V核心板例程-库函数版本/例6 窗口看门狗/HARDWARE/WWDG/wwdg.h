#ifndef _WWDG_H
#define _WWDG_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 

// STM32F407开发板
//看门狗 驱动代码	   

////////////////////////////////////////////////////////////////////////////////// 	 

void WWDG_Init(u8 tr,u8 wr,u32 fprer);
void WWDG_IRQHandler(void);
#endif
