STM32F4开发板
	

	学习STM32内部DAC的使用
	
	串口1(波特率:115200,PA9/PA10)
	1.8寸TFTLCD模块(通过SPI驱动) 
	按键KEY_UP(PA0,也称之为WK_UP)
	DAC(STM32内部DAC通道1,通过PA4输出)
	ADC(STM32内部ADC1_CH5,连接在PA5).


	本实验利用按键（KEY_UP按键）控制STM32F4内部DAC模块的通道1来输出电压，通过ADC1的
	通道5采集DAC的输出电压，在LCD模块上面显示ADC获取到的电压值以及DAC的设定输出电压值等信息。同时可以通
	过usmart调用Dac1_Set_Vol函数，来直接设置DAC输出电压。
	

	ADC和DAC的参考电压默认通过P7连接在VDDA上面(3.3V),所以默认参考电压是3.3V  
	通过跳线帽短接多功能接口(P12)的ADC和DAC,即可实现自测试(AD读取DA值,显示在LCD上) 
	

					
					
					
					
					
					
					
					
					
					
					
					
					
					