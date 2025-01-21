STM32F4开发板
	
	学习STM32 SPI接口的使用,驱动W25Q16实现SPI FLASH数据读写.
	

	串口1(波特率:115200,PA9/PA10)
	1.8寸TFTLCD模块(通过FSMC驱动,FSMC_NE4接LCD片选/A6接RS) 
	按键K1
	SPI1(PB3/4/5/)  CS(PA15)
	W25Q128(SPI FLASH芯片,连接在SPI1上)


	本实验通过K1按键来控制W25Q16的写入后，再进行读取。并在LCD模块
	上面显示相关信息。D2提示程序正在运行。
	  

					
					
					
					
					
					
					
					
					
					