
#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"

//STM32F407开发板 
//STM32F4工程模板-库函数版本


int main(void)
{
	u32 t=0;
	uart_init(115200);
	delay_init(84);
	
  while(1){
    printf("t:%d\r\n",t);
		delay_ms(500);
		t++;
	}
}




