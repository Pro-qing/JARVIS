STM32F4开发板
	
 STM32的USB SLAVE功能使用,实现一个简单的USB读卡器
	
硬件资源:
	1,D2(连接在PA1)
	2,串口1(波特率:115200,PA9/PA10)
	3,1.8寸TFTLCD模块(通过SPI驱动) 
	4,SD卡,通过SDIO(SDIO_D0~D4(PC8~PC11),SDIO_SCK(PC12),SDIO_CMD(PD2))连接 
	5,W25Q16(SPI FLASH芯片,连接在SPI1上)
	6,USB_SLAVE接口(将D-/D+连接在PA11/PA12上)

实验现象:
	本实验代码,开机的时候先检测SD卡和SPI FLASH是否存在，如果存在则获取其容量，并显示在LCD上
	面（如果不存在，则报错）。之后开始USB配置，在配置成功之后就可以在电脑上发现两个可移动磁
	盘。我们用D2来指示USB正在读写，并在液晶上显示出来，指示程序正在运行。


					
					
					
					
					
					
					
					
					
					
					
					