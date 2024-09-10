#include <ros/ros.h>
#include <std_msgs/String.h>
#include <cmath>
#include <string.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <geometry_msgs/PoseStamped.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <serial/serial.h>

enum mode
{
	mission = 0,
	offboard = 1,
	choose = 2,
	bombing = 3,
};
mode mission_mode = mission;


float yaw=0,x=0,y=0,z=0,w=0;

mavros_msgs::State current_state;
geometry_msgs::PoseStamped local_pos;
geometry_msgs::PoseStamped local_pos_transfer;

void state_sb(const mavros_msgs::State::ConstPtr& msg) {
	current_state = *msg;
}
void local_pos_sb(const geometry_msgs::PoseStamped::ConstPtr& msg) {
	local_pos = *msg;
}

int choose_s(int a) {
	if (a > 0 && a < 10) return 1;
	if (a > 10 && a < 100) return 2;
	if (a > 100 && a < 1000) return 3;
}

std::string port = "/dev/ttyUSB0"; // 根据实际情况修改串口设备号
int baudrate = 115200; // 根据实际情况修改波特率
serial::Serial serialPort(port, baudrate, serial::Timeout::simpleTimeout(1000));

int main(int argc, char** argv) {
	ros::init(argc, argv, "left");
	ros::NodeHandle nh;
	ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>("/mavros/state", 10, state_sb);
	ros::Subscriber plane_local_pos_sub = nh.subscribe<geometry_msgs::PoseStamped>("/mavros/local_position/pose", 1, local_pos_sb);
	ros::Publisher target_pos_pub = nh.advertise<geometry_msgs::PoseStamped>("/mavros/setpoint_position/local", 10);
	ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>("/mavros/cmd/arming");
	ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("/mavros/set_mode");
	ros::Rate rate(60.0);
	ros::spinOnce();
	rate.sleep();

	geometry_msgs::PoseStamped pose;
	pose.pose.position.x = -46.500;
	pose.pose.position.y = 126.800;
	pose.pose.position.z = 20;


	while (ros::ok() && !current_state.connected)
	{
		ROS_INFO("not connected");
		ros::spinOnce();
		rate.sleep();
	}
	for (int i = 100; ros::ok() && i > 0; --i) {
		pose.header.stamp = ros::Time::now();
		target_pos_pub.publish(pose);
		ros::spinOnce();
		rate.sleep();
	}

	mavros_msgs::SetMode offb_set_mode;
	offb_set_mode.request.custom_mode = "AUTO.MISSION";
	mavros_msgs::CommandBool arm_cmd;
	arm_cmd.request.value = true;
	ros::Time last_request = ros::Time::now();

	int count = 0;
	int cnt = 0;
	int ok = 0;
	float error_x = 0;
	float error_y = 0;
	float distance = 0;
	int target_c = 0;

	std_msgs::String msg;
	msg.data = '1';//舵机控制

	while (ros::ok()) {
		pose.header.stamp = ros::Time::now();
		target_pos_pub.publish(pose);

		if (current_state.mode == "STABILIZED") {
			ROS_INFO("switch to STABILIZED");
			std::exit(0);
		}//退出机制
		//ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << local_pos);

		switch (mission_mode) {
		case 0:{
			while(cnt==0&&count<5){
			if (!current_state.armed && ok == 0) {
				pose.header.stamp = ros::Time::now();
				target_pos_pub.publish(pose);
				if (arming_client.call(arm_cmd) && arm_cmd.response.success) {
					ROS_INFO("Vehicle armed");
					
				}
				last_request = ros::Time::now();
			}


			if (current_state.mode != "AUTO.MISSION") {
				if (set_mode_client.call(offb_set_mode) && offb_set_mode.response.mode_sent) {
					ROS_INFO("MISSION enabled");
				}
				else  ROS_INFO("MISSION disabled");
			

			}count++;}
			
			if(cnt==2){
				for(int i =1;i<5;i++){
				if (current_state.mode != "AUTO.MISSION") {
				if (set_mode_client.call(offb_set_mode) && offb_set_mode.response.mode_sent) {
					ROS_INFO("MISSION enabled");
				}
				else  ROS_INFO("MISSION disabled"); }}
				std::exit(0);
			}

			if(cnt==0){
				std::ifstream afile("/home/amov/Desktop/target_left.txt");
				if (afile.is_open()) { // 从文件中读取数据
					int intValue;
					float floatValue1, floatValue2;
					std::cout<<std::endl;
					while (afile >> intValue >> floatValue1 >> floatValue2)
					{
						std::cout << intValue << " " << floatValue1 << " " << floatValue2 << std::endl;
					}
					afile.close();
				}
				else
				{
					std::cerr << "无法打开文件进行读取。" << std::endl;
				}
				std::cout << "Target:";
				std::cin >> target_c;
				while (target_c > 1000) {
					std::cin >> target_c;
				}
				target_c = choose_s(target_c);
				std::ifstream bfile("/home/amov/Desktop/target_left.txt");
				int intValue=0;
				float floatValue1, floatValue2;
				while (bfile >> intValue >> floatValue1 >> floatValue2 ){
					std::cout << intValue << " " << floatValue1 << " " << floatValue2 << std::endl;
					intValue ++;
					if(intValue == target_c+1) break;
					}
		
				pose.pose.position.x = floatValue1;
				pose.pose.position.y = floatValue2;
				pose.pose.position.z = 20;
				ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << pose);
				cnt++;
			}
			if(local_pos.pose.position.z > 33 && cnt==1){
				ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << pose);
				offb_set_mode.request.custom_mode = "OFFBOARD";
				mission_mode = offboard;
				cnt=2;}
			else{ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << local_pos);
			     ros::spinOnce();
			     rate.sleep();
			     continue;
	}
			break;}
		
		case 1: {
			if (current_state.mode != "OFFBOARD") {
				if (set_mode_client.call(offb_set_mode) && offb_set_mode.response.mode_sent) {
					ROS_INFO("Offboard enabled");
				}
				else { ROS_INFO("Offboard disabled"); }
			}

			rate.sleep();

			pose.header.stamp = ros::Time::now();
			target_pos_pub.publish(pose);

			float distance = sqrt((local_pos.pose.position.x - pose.pose.position.x) * (local_pos.pose.position.x -  pose.pose.position.x) + (local_pos.pose.position.y -pose.pose.position.y) * (local_pos.pose.position.y - pose.pose.position.y));
			//ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << distance_s[3]);
			ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << distance);
			if (distance < 20) {
					ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << local_pos);
					serialPort.write(msg.data);
					mission_mode = mission;
					offb_set_mode.request.custom_mode = "AUTO.MISSION";
					last_request = ros::Time::now();
			}

		}
		default: {
			last_request = ros::Time::now();
			break; }
		}
		ros::spinOnce();
		rate.sleep();}	
		return 0;
}
