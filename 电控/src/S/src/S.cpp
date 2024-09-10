#include <ros/ros.h>
#include <std_msgs/String.h>
#include <string>
#include <iostream>
#include <serial/serial.h>

std::string port = "/dev/ttyUSB0"; // 根据实际情况修改串口设备号
int baudrate = 115200; // 根据实际情况修改波特率
serial::Serial serialPort(port, baudrate, serial::Timeout::simpleTimeout(1000));

int main(int argc, char** argv)
{
    ros::init(argc, argv, "servo");
    ros::NodeHandle nh;
    ros::Rate loop_rate(10);
    int count = 0;
    while (ros::ok())
    {
	int choice = 0;
	std::cin>>choice;
	if(choice == 1){        // 创建字符串消息对象
        std_msgs::String msg;       
        msg.data = '1';
	serialPort.write(msg.data);
        ROS_INFO("%s", msg.data.c_str());
} //control the serve
	
	if(choice == 0){        // 创建字符串消息对象
        std_msgs::String msg;       
        msg.data = '0';
	serialPort.write(msg.data);
        ROS_INFO("%s", msg.data.c_str());
} //control the serve

	if(choice == 2){
	exit(0);	
}
		

        // 等待下一次循环
        loop_rate.sleep();
    
    }
    ros::spin();
    return 0;
}


