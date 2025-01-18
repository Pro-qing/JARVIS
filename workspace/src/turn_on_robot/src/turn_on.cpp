#include "robot.h"


//实例化IMU对象
sensor_msgs::Imu Mpu6050;

/***************************************
功能: 主函数，ROS初始化，通过turn_on类创建Robot_control对象并自动调用构造函数初始化
***************************************/
int main(int argc,char** argv)
{
    //ROS初始化 并设置节点名称JAVRS_robot
    ros::init(argc, argv, "JARVIS_robot");
    //实例化一个对象
    turn_on Robot_Control;
    //循环执行数据采集和发布话题等操作
    Robot_Control.Control();
    return 0;
}

/**************************************
功能: 数据转换函数
***************************************/
short turn_on::IMU_Trans(uint8_t Data_High,uint8_t Data_Low)
{
    short transition_16;
    transition_16 = 0;
    transition_16 |= Data_High<<8;
    transition_16 |= Data_Low;
    return transition_16;
}

float turn_on::Odom_Trans(uint8_t Data_High,uint8_t Data_Low)
{
    float data_return;
    short transition_16;
    transition_16 = 0;
    transition_16 |= Data_High<<8;
    transition_16 |= Data_Low;
    data_return = (transition_16 / 1000) + (transition_16 % 1000) *0.001;
    return data_return;
}

/**************************************
功能: 速度话题订阅回调函数Callback，根据订阅的指令通过串口发指令控制下位机
***************************************/
void turn_on::Cmd_Vel_Callback(const geometry_msgs::Twist::Twist &twist_aux)
{
    //中间变量
    short transition;

    
}

