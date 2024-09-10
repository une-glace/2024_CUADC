#include <ros/ros.h>
#include <std_msgs/String.h>
#include <cmath>
#include <string.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <geometry_msgs/PoseStamped.h>
#include <fstream>
#include <iostream>
#include <string>
#include <serial/serial.h>

enum mode
{
	mission = 0,
	offboard = 1,
	choose = 2,
	bombing = 3,
};
mode mission_mode = mission;

float matrix[2][2] = { 1, 0, 0, 1 };
float inves_matrix[2][2] = { 1, 0, 0, 1 };//机身坐标系与dbt坐标系的转换
bool Mat = 1;

mavros_msgs::State current_state;
geometry_msgs::PoseStamped local_pos;
geometry_msgs::PoseStamped local_pos_transfer;

inline void position_transfer_write(geometry_msgs::PoseStamped local_pos) {
	local_pos_transfer.pose.position.x = matrix[0][0] * local_pos.pose.position.x + matrix[0][1] * local_pos.pose.position.y;
	local_pos_transfer.pose.position.y = matrix[1][0] * local_pos.pose.position.x + matrix[1][1] * local_pos.pose.position.y;
	float data[] = { local_pos_transfer.pose.position.x, local_pos_transfer.pose.position.y };
	std::ofstream outFile("/home/binomialdist/Desktop/position.txt", std::ofstream::out | std::ofstream::trunc);
	if (!outFile.is_open())
	{
		std::cerr << "无法打开文件" << std::endl;
	}
	else
	{
		outFile << local_pos_transfer.pose.position.x << " " << local_pos_transfer.pose.position.y << " " << local_pos_transfer.pose.position.z << std::endl;
		ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << local_pos_transfer);
			outFile.close();
	}
} //坐标转化与写入

void state_sb(const mavros_msgs::State::ConstPtr& msg) {
	current_state = *msg;
}
void local_pos_sb(const geometry_msgs::PoseStamped::ConstPtr& msg) {
	local_pos = *msg;
	position_transfer_write(*msg);
}
//回调函数

std::string port = "/dev/ttyUSB0"; // 根据实际情况修改串口设备号
int baudrate = 115200; // 根据实际情况修改波特率
serial::Serial serialPort(port, baudrate, serial::Timeout::simpleTimeout(1000));
//舵机

int main(int argc, char** argv) {
	ros::init(argc, argv, "rise");
	ros::NodeHandle nh;
	ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>("/plane_0/mavros/state", 10, state_sb);
	ros::Subscriber plane_local_pos_sub = nh.subscribe<geometry_msgs::PoseStamped>("/plane_0/mavros/local_position/pose", 1, local_pos_sb);
	ros::Publisher target_pos_pub = nh.advertise<geometry_msgs::PoseStamped>("/plane_0/mavros/setpoint_position/local", 10);
	ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>("/plane_0/mavros/cmd/arming");
	ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("/plane_0/mavros/set_mode");
	ros::Rate rate(20.0);
	ros::spinOnce();
	rate.sleep();

	float target[3][5] = { 100,0,30,  100,200,30,   0,300,30,   0,-100,25 };//目标航点坐标
	float origin_yaw[4] = {};
	geometry_msgs::PoseStamped pose, pos_after_transfer;
	pose.pose.position.x = target[0][0];
	pose.pose.position.y = target[1][0];
	pose.pose.position.z = target[2][0];


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
	msg.data = '3';//舵机控制

	while (ros::ok()) {
		pose.header.stamp = ros::Time::now();
		target_pos_pub.publish(pose);

		if (current_state.mode == "STABILIZED") {
			ROS_INFO("switch to STABILIZED");
			std::exit(0);
		}//退出机制
		ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << local_pos);

		switch (mission_mode) {
		case 0: {
			if (!current_state.armed && ok == 0) {
				pose.header.stamp = ros::Time::now();
				target_pos_pub.publish(pose);
				if (arming_client.call(arm_cmd) && arm_cmd.response.success) {
					ROS_INFO("Vehicle armed");
					origin_yaw[0] = local_pos.pose.orientation.w;
					origin_yaw[1] = local_pos.pose.orientation.x;
					origin_yaw[2] = local_pos.pose.orientation.y;
					origin_yaw[3] = local_pos.pose.orientation.z;
					float x = origin_yaw[0];
					float y = origin_yaw[1];
					float z = origin_yaw[2];
					float w = origin_yaw[3];

					float yaw = atan2(2.0 * (x * w + y * z), 1 - 2 * (z * z + w * w));
					float rotation_matrix[2][2];
					matrix[0][0] = cos(yaw);
					matrix[0][1] = -sin(yaw);
					matrix[1][0] = sin(yaw);
					matrix[1][1] = cos(yaw);
					float yaw_s = -yaw;
					inves_matrix[0][0] = cos(yaw_s);
					inves_matrix[0][1] = -sin(yaw_s);
					inves_matrix[1][0] = sin(yaw_s);
					inves_matrix[1][1] = cos(yaw_s);
				}
				last_request = ros::Time::now();
			}
			else if (current_state.armed && ok == 0) {
				if (current_state.mode != "AUTO.MISSION") {
					pose.header.stamp = ros::Time::now();
					target_pos_pub.publish(pose);
					if (set_mode_client.call(offb_set_mode) && offb_set_mode.response.mode_sent) {
						ROS_INFO("Mission enabled");
					}
				}
				if (cnt == 0 && local_pos.pose.position.z > 35) {
					ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << pose);
					offb_set_mode.request.custom_mode = "OFFBOARD";
					mission_mode = offboard;
					cnt++;
				}
			}
			else if (ok == 1 && local_pos.pose.position.z > 0) {
				if (current_state.mode != "AUTO.MISSION") {
					pose.header.stamp = ros::Time::now();
					target_pos_pub.publish(pose);
					if (set_mode_client.call(offb_set_mode) && offb_set_mode.response.mode_sent) {
						ROS_INFO("Mission enabled");
					}
				}
			}
			else if (ok == 1 && local_pos.pose.position.z <= 0) {
				ROS_INFO("Vehicle disarmed");
				ok == 2;
			}
			break; }

		case 1: {
			if (current_state.mode != "OFFBOARD") {
				if (set_mode_client.call(offb_set_mode) && offb_set_mode.response.mode_sent) {
					ROS_INFO("Offboard enabled");
				}
				else { ROS_INFO("Offboard disabled"); }
			}

			error_x = target[0][0] - local_pos.pose.position.x;
			error_y = target[1][0] - local_pos.pose.position.y;
			distance = pow(error_x, 2) + pow(error_y, 2);
			pose.pose.position.x = target[0][0] + error_x / distance / 10;
			pose.pose.position.y = target[1][0] + error_y / distance / 10;
			ros::spinOnce();
			rate.sleep();
			pose.header.stamp = ros::Time::now();
			target_pos_pub.publish(pose);
			int distance_s[5] = { sqrt((local_pos.pose.position.x - target[0][0]) * (local_pos.pose.position.x - target[0][0]) + (local_pos.pose.position.y - target[1][0]) * (local_pos.pose.position.y - target[1][0])),
				sqrt((local_pos.pose.position.x - target[0][1]) * (local_pos.pose.position.x - target[0][1]) + (local_pos.pose.position.y - target[1][1]) * (local_pos.pose.position.y - target[1][1])),
				sqrt((local_pos.pose.position.x - target[0][2]) * (local_pos.pose.position.x - target[0][2]) + (local_pos.pose.position.y - target[1][2]) * (local_pos.pose.position.y - target[1][2])),
				sqrt((local_pos.pose.position.x - target[0][3]) * (local_pos.pose.position.x - target[0][3]) + (local_pos.pose.position.y - target[1][3]) * (local_pos.pose.position.y - target[1][3])),
				sqrt((local_pos.pose.position.x - target[0][4]) * (local_pos.pose.position.x - target[0][4]) + (local_pos.pose.position.y - target[1][4]) * (local_pos.pose.position.y - target[1][4])) }

			ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << distance_s);

			if (count == 0 && distance_s[0] < 5) {
				ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m complete1\033[0m");
				pose.pose.position.x = target[0][1];
				pose.pose.position.y = target[1][1];
				count++;
			}
			if (count == 1 && distance_s[1] < 5) {
				ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m complete2\033[0m");
				pose.pose.position.x = target[0][2];
				pose.pose.position.y = target[1][2];
				count++;
			}
			if (count == 2 && distance_s[2] < 5) {
				ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m complete3\033[0m");
				pose.pose.position.x = target[0][3];
				pose.pose.position.y = target[1][3];
				count++;
			}
			if (count == 3 && distance_s[1] < 5) {
				ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m complete3\033[0m");
				pose.pose.position.x = target[0][2];
				pose.pose.position.y = target[1][2];
				count++;
			}
			if (count == 4) {
				std::ifstream file("/home/binomialdist/Desktop/target.txt");
				if (file.is_open()) { // 从文件中读取数据
					int intValue;
					float floatValue1, floatValue2;
					while (file >> intValue >> floatValue1 >> floatValue2)
					{
						std::cout << intValue << " " << floatValue1 << " " << floatValue2 << std::endl;
					}
					file.close();
				}
				else
				{
					std::cerr << "无法打开文件进行读取。" << std::endl;
				}

				mission_mode = choose;
			}



			break; }

		case 2: {
			std::cout << "Please choose the target:";
			std::cin >> target_c;
			std::ifstream file("/home/binomialdist/Desktop/target.txt");
			int intValue;
			float floatValue1, floatValue2;
			do {
				std::cout << intValue << " " << floatValue1 << " " << floatValue2 << std::endl;
				file.close();
			} while (file >> intValue >> floatValue1 >> floatValue2 && intValue != target_c);

			pose.pose.position.x = floatValue1;
			pose.pose.position.y = floatValue2;
			pose.pose.position.x = pose.pose.position.x * inves_matrix[0][0] + pose.pose.position.y * inves_matrix[0][1];
			pose.pose.position.y = pose.pose.position.x * inves_matrix[1][0] + pose.pose.position.y * inves_matrix[1][1];
			target[0][3] = pose.pose.position.x;
			target[1][3] = pose.pose.position.y;
			mission_mode = bombing;
			count++;
			break; }


		case 3: {
			pose.header.stamp = ros::Time::now();
			target_pos_pub.publish(pose);
			float distance_pose = sqrt((local_pos.pose.position.x - target[0][3]) * (local_pos.pose.position.x - target[0][3]) + (local_pos.pose.position.y - target[1][3]) * (local_pos.pose.position.y - target[1][3]));
				if (distance_pose < 8) {
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
		if (ok == 2) {
			std::exit(0);
		}
		ros::spinOnce();
		rate.sleep();}	
		return 0;
}
