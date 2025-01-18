#ifndef __ROBOT_H_
#define __ROBOT_H_

#include "ros/ros.h"
#include <iostream>
#include <string.h>
#include <string> 
#include <math.h>
#include <stdlib.h>    
#include <unistd.h>      
#include <sys/types.h>
#include <sys/stat.h>
#include <serial/serial.h>
#include <fcntl.h>          
#include <stdbool.h>
#include <tf/transform_broadcaster.h>
#include <std_msgs/String.h>
#include <std_msgs/Float32.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <sensor_msgs/Imu.h>
using namespace std;

//宏定义
#define SEND_DATA_CHECK         1                 //发送数据校验
#define RECEIVE_DATA_CHECK      0                 //接收数据校验 
#define FRAME_HEADER            0xAA              //帧头
#define FRAME_END               0xBB              //帧尾
#define RECEIVE_DATA_SIZE       24                //接收缓存长度
#define SEND_DATA_SIZE          24                //发送缓存长度
#define PI 				        3.1415926f        //PI //圆周率

//IMU陀螺仪设置，量程 -500° ~ +500° 
//陀螺仪原始数据转为弧度制（rad）0.00026644f = 1/360*2*PI
#define  GYROSCOPE_RATIO        0.00026644f       //陀螺仪比例系数

//IMU加速度计设置，量程 -2g ~ +2g 
//加速度计原始数据转为m/s^2,
#define ACCEL_RATIO             1671.84f         //加速度比例系数

//协方差矩阵，用于里程计话题数据，用于robot_localization功能包
//robot_localization功能包不需要在这里设置，在yamla文件中设置协方差矩阵，

//速度和位置的数据结构体，二者共用一个
typedef struct 
{
    float x;
    float y;
    float z;
}vel_pos_data;

//IMU数据结构体
typedef struct
{
    short roll_x;
    short pitch_y;
    short yaw_z;
    short accel_x;
    short accel_y;
    short accel_z;
}imu_data;

//ROS->STM32
typedef struct 
{
    uint8_t tx[SEND_DATA_SIZE];
    float x_speed;
    float y_speed;
    float z_speed;
    unsigned char Frame_end;
}send_data;

//STM32->ROS
typedef struct 
{
    uint8_t rx[RECEIVE_DATA_SIZE];
    uint8_t Flag_Stop;
    unsigned char Frame_header;
    float x_speed;
    float y_speed;
    float z_speed;
    //float Power_Voltage;
    unsigned char Frame_end;
}receive_data;

//机器人底盘类
class turn_on
{
    public:
        turn_on();              //构造函数
        ~turn_on();             //析构函数
        void Control();         //控制函数
        serial::Serial ttyUSB-stm32;    //串口对象
        serial::Serial ttyUSB-imu;      //串口对象
    private:
        ros::NodeHandle n;              //创建ROS节点句柄
        ros::Timer _Now,_Last_Time;     //时间相关，计算里程
        float Samling_Time;             //定义采样时间，计算里程

        ros::Subscriber Cmd_Vel_Sub;    //初始化话题订阅者
        //速度话题订阅回调函数
        void Cmd_Vel_Callback(const geometry_msgs::Twist &twist_aux);

        //初始化话题发布者
        ros::Publisher odom_publisher,imu_publisher,volatile_publisher;
        void Pulisher_odom();
        void Pulisher_imu();
        void Pulisher_volatile();

        //串口（ttyUSB-imu）读取信息
        bool Get_IMU_Data();
        //串口（ttyUSB-stm32）读取信息,读取电压，底盘速度
        bool Get_Sensor_Data();
        bool Get_Sensor_Data_New();
        float Odom_Trans(uint8_t Data_High,uint8_t Data_Low);//里程计数据转化读取
        unsigned char Check_Sum(unsigned char Count_Number,unsigned char mode)//BBC校验函数

        string usart_port_name, robot_frame_id, gyro_frame_id, odom_frame_id; //Define the related variables //定义相关变量
        int serial_baud_rate;                                   //定义串口波特率
        receive_data Receive_data;                              //串口接收数据
        send_data Send_data;                                    //串口发送数据

        vel_pos_data robot_pos;                                 //机器人位置
        vel_pos_data robot_vel;                                 //机器人速度
        imu_data imu_data;                                      //IMU数据

        float Voltage;                                          //电压数据
        float odom_x,odom_y,odom_z_positive,odom_z_negative;    //里程计修正参数
};
#endif
