/***********************************************
公司：轮趣科技（东莞）有限公司
品牌：WHEELTEC
官网：wheeltec.net
淘宝店铺：shop114407458.taobao.com 
速卖通: https://minibalance.aliexpress.com/store/4455017
版本：V5.0
修改时间：2021-11-05

Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version: V5.0
Update：2021-11-05

All rights reserved
***********************************************/

#include "system.h"

//Robot software fails to flag bits
//机器人软件失能标志位
u8 Flag_Stop=1;   

//The ADC value is variable in segments, depending on the number of car models. Currently there are 6 car models
//ADC值分段变量，取决于小车型号数量，目前有6种小车型号
int Divisor_Mode;

// Robot type variable
//机器人型号变量
u8 Car_Mode=0; 

//Default speed of remote control car, unit: mm/s
//遥控小车的默认速度，单位：mm/s
float RC_Velocity=500; 

//Vehicle three-axis target moving speed, unit: m/s
//Note that Move_Y does not work and only makes sense for omnidirectional moving cars
//小车三轴目标运动速度，单位：m/s
//注意Move_Y不起作用，只有对全向移动小车Move_Y才有意义
float Move_X, Move_Y, Move_Z;   

//Steering gear control PWM value, used to control the front wheel Angle, Ackerman car special
//舵机控制PWM值，用于控制前轮转角，阿克曼小车专用
int Servo=1600; 

//PID parameters of Speed control
//速度控制PID参数
float Velocity_KP=300,Velocity_KI=300; 

//Smooth control of intermediate variables, dedicated to omni-directional moving cars
//平滑控制中间变量，全向移动小车专用
Smooth_Control smooth_control;  

//The parameter structure of the motor
//电机的参数结构体
Motor_parameter MOTOR_A,MOTOR_B;  

/************ 小车型号相关变量 **************************/
/************ Variables related to car model ************/
//Encoder accuracy
//编码器精度
float Encoder_precision; 
//Wheel circumference, unit: m
//轮子周长，单位：m
float Wheel_perimeter; 
//Drive wheel base, unit: m
//主动轮轮距，单位：m
float Wheel_spacing; 
//The wheelbase of the front and rear axles of the trolley, unit: m
//小车前后轴的轴距，单位：m
float Axle_spacing; 

/************ 小车型号相关变量 **************************/
/************ Variables related to car model ************/

//PS2 controller, Bluetooth APP, aircraft model controller, CAN communication, serial port 1 communication control flag bit.
//These 5 flag bits are all 0 by default, representing the serial port 3 control mode
//PS2手柄、蓝牙APP、航模手柄、CAN通信、串口1通信控制标志位。这5个标志位默认都为0，代表串口3控制模式
u8 PS2_ON_Flag=0, APP_ON_Flag=0, Remote_ON_Flag=0, CAN_ON_Flag=0, Usart_ON_Flag=0; 

//Bluetooth remote control associated flag bits
//蓝牙遥控相关的标志位
u8 Flag_Left, Flag_Right, Flag_Direction=0, Turn_Flag; 

//Sends the parameter's flag bit to the Bluetooth APP
//向蓝牙APP发送参数的标志位
u8 PID_Send; 

//The PS2 gamepad controls related variables
//PS2手柄控制相关变量
float PS2_LX,PS2_LY,PS2_RX,PS2_RY,PS2_KEY; 

void systemInit(void)
{
	//Turn off the JTAG interface to enable the OLED display
	//关闭JTAG接口，关闭JTAG接口才能开启OLED显示屏
//	JTAG_Set(JTAG_SWD_DISABLE); 
	
	//Open the SWD interface debug interface
	//开启SWD接口调试接口
//	JTAG_Set(SWD_ENABLE);           
	
	//Interrupt priority group setting
	//中断优先级分组设置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	//Delay function initialization
	//延时函数初始化
	delay_init(168);			                                            
	
	//Initialize the hardware interface connected to the LED lamp
	//初始化与LED灯连接的硬件接口
	LED_Init();   
	
	//Initialize the hardware interface connected to the enable switch
	//初始化与使能开关连接的硬件接口
	Enable_Pin();

  //Initialize the hardware interface connected to the OLED display
  //初始化与OLED显示屏连接的硬件接口	
	OLED_Init();     
	
	//Initialize the hardware interface connected to the user's key
	//初始化与用户按键连接的硬件接口
	KEY_Init();	
	
	//Serial port 1 initialization, communication baud rate 115200, 
	//can be used to communicate with ROS terminal
	//串口1初始化，通信波特率115200，可用于与ROS端通信
	uart1_init(115200);	  
	
	//Serial port 2 initialization, communication baud rate 9600, 
	//used to communicate with Bluetooth APP terminal
	//串口2初始化，通信波特率9600，用于与蓝牙APP端通信
	uart2_init(9600);  
	
	//Serial port 3 is initialized and the baud rate is 115200. 
	//Serial port 3 is the default port used to communicate with ROS terminal
	//串口3初始化，通信波特率115200，串口3为默认用于与ROS端通信的串口
	uart3_init(115200);

  //Initialize the CAN communication interface
  //CAN通信接口初始化
//	CAN1_Mode_Init(1,2,3,6,0); 
  CAN1_Mode_Init(1,3,3,6,0); 
	//ADC pin initialization, used to read the battery voltage and potentiometer gear, 
	//potentiometer gear determines the car after the boot of the car model
	//ADC引脚初始化，用于读取电池电压与电位器档位，电位器档位决定小车开机后的小车适配型号
 	Adc_Init();  
  //According to the tap position of the potentiometer, determine which type of car needs to be matched, 
  //and then initialize the corresponding parameters	
  //根据电位器的档位判断需要适配的是哪一种型号的小车，然后进行对应的参数初始化	
	Robot_Select(); 
	
	//Encoder A is initialized to read the real time speed of motor A
	//编码器A初始化，用于读取电机A的实时速度
	Encoder_Init_TIM2(); 
	//Encoder B is initialized to read the real time speed of motor B
  //编码器B初始化，用于读取电机B的实时速度	
	Encoder_Init_TIM3();  
	
	//Initialize the motor direction control pin, 
	//which is used to control the motor forward and reverse rotation
	//初始化电机方向控制引脚，用于控制电机正反转
	MiniBalance_Motor_Init();  

  //Initialize motor speed control and, for controlling motor speed, PWM frequency 10kHz
  //初始化电机速度控制以及，用于控制电机速度，PWM频率10KHZ
//	MiniBalance_PWM_Init(7199,0);  
    MiniBalance_PWM_Init(8399,0);
	
	//Initialize PWM control steering gear pin, PWM frequency 50Hz
	//初始化PWM控制舵机引脚，	PWM频率50HZ
//	Servo_PWM_Init(9999,71);   
    Servo_PWM_Init(9999,83);
	
  //IIC initialization for MPU9250
  //IIC初始化，用于MPU9250
	IIC_Init();
	
	//MPU9250 is initialized to read the vehicle's three-axis attitude, 
	//three-axis angular velocity and three-axis acceleration information
  //MPU9250初始化，用于读取小车三轴姿态、三轴角速度、三轴加速度信息
  Flag_Mpu6050=MPU6050_Init();
	
	//Initialize the model aircraft remote control interface. 
	//Note that the pins of serial port 1 and model aircraft remote control conflict. 
	//When opened at the same time, serial port 1 will send data wrong
	//初始化航模遥控接口，要注意的是串口1和航模遥控的引脚冲突，同时开启时，串口1发送数据会错误
  TIM1_Cap_Init(0XFFFF,168-1);     
	
	//Initialize the hardware interface to the PS2 controller
	//初始化与PS2手柄连接的硬件接口
	PS2_Init();
	
	//PS2 gamepad configuration is initialized and configured in analog mode
  //PS2手柄配置初始化,配置为模拟量模式	
	PS2_SetInit();
}
