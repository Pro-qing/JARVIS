STM32F4开发板
	
实验目的:
	学习STM32内部ADC的使用
	
       串口1(波特率:115200,PA9/PA10)
       1.8寸TFTLCD模块(通过FSMC驱动,FSMC_NE4接LCD片选/A6接RS) 
       ADC(STM32内部ADC1,通道5,即:ADC1_CH5)

      本实验通过STM32内部ADC1读取通道5（PA5）上面的电压，在LCD模块上面显示ADC转换值以及换算成电压后的电压值。
	

					
					
					
					
					
					
					
					
					
					
					