// #include <ros/ros.h>
// #include <std_msgs/String.h>
// #include <cmath>
// #include <string.h>
// #include <mavros_msgs/CommandBool.h>
// #include <mavros_msgs/SetMode.h>
// #include <mavros_msgs/State.h>
// #include <geometry_msgs/PoseStamped.h>
// #include <fstream>
// #include <sstream>
// #include <iostream>
// #include <string>
// #include <vector>
// #include <serial/serial.h>

// enum mode
// {
// 	mission = 0,
// 	offboard = 1,
// 	choose = 2,
// 	bombing = 3,
// };
// mode mission_mode = bombing;

// mavros_msgs::State current_state;
// geometry_msgs::PoseStamped local_pos;

// void state_sb(const mavros_msgs::State::ConstPtr& msg) {
// 	current_state = *msg;
// 	ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << current_state);
// }
// void local_pos_sb(const geometry_msgs::PoseStamped::ConstPtr& msg) {
// 	local_pos = *msg;
	
// }

// // std::string port = "/dev/ttyUSB0"; // 根据实际情况修改串口设备号
// // int baudrate = 115200; // 根据实际情况修改波特率
// // serial::Serial serialPort(port, baudrate, serial::Timeout::simpleTimeout(1000));



// int main(int argc, char** argv) {
//     ros::init(argc, argv, "mission_bombing");
// 	ros::NodeHandle nh;
//     ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>("/mavros/state", 10, state_sb);
// 	ros::Subscriber plane_local_pos_sub = nh.subscribe<geometry_msgs::PoseStamped>("/mavros/local_position/pose", 1, local_pos_sb);
// 	ros::Publisher target_pos_pub = nh.advertise<geometry_msgs::PoseStamped>("/mavros/setpoint_position/local", 10);
// 	ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>("/mavros/cmd/arming");
// 	ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("/mavros/set_mode");
// 	ros::Rate rate(60.0);
// 	ros::spinOnce();
// 	rate.sleep();
	
// 	geometry_msgs::PoseStamped pose;
// 	pose.pose.position.x = -50;
// 	pose.pose.position.y = 50;
// 	pose.pose.position.z = 40;
//     while (ros::ok() && !current_state.connected)
// 	{
// 		ROS_INFO("not connected");
// 		ros::spinOnce();
// 		rate.sleep();
// 	}
// 	for (int i = 100; ros::ok() && i > 0; --i) {
// 		pose.header.stamp = ros::Time::now();
// 		target_pos_pub.publish(pose);
// 		ros::spinOnce();
// 		rate.sleep();
// 	}

// 	mavros_msgs::SetMode offb_set_mode;
// 	offb_set_mode.request.custom_mode = "AUTO.MISSION";
// 	mavros_msgs::CommandBool arm_cmd;
// 	arm_cmd.request.value = true;
// 	ros::Time last_request = ros::Time::now();

//     std_msgs::String msg;
// 	msg.data = '1';//舵机控制

//     int ok=0;
	


//     while (ros::ok()) {
//         if (current_state.mode == "STABILIZED") {
// 			ROS_INFO("switch to STABILIZED");
// 			std::exit(0);
// 		}
//         switch (mission_mode) {
//             case 0: 
// 			if (!current_state.armed) {
// 				pose.header.stamp = ros::Time::now();
				
// 				if (arming_client.call(arm_cmd) && arm_cmd.response.success) {
// 					ROS_INFO("Vehicle armed");
					
// 				}
// 				last_request = ros::Time::now();
// 			}
//             else if (current_state.armed&&ok<5) {
// 				if (current_state.mode != "AUTO.MISSION") {
// 					pose.header.stamp = ros::Time::now();
// 					target_pos_pub.publish(pose);
// 					if (set_mode_client.call(offb_set_mode) && offb_set_mode.response.mode_sent) {
// 						ROS_INFO("Mission enabled");
// 						ok++;
// 					}
// 				}}
// 			else if(ok==5){
// 				mode mission_mode = bombing;
// 			}
// 			break;

// 			case 3:
				
// 				float distance =sqrt((local_pos.pose.position.x-pose.pose.position.x)*(local_pos.pose.position.x-pose.pose.position.x)+(local_pos.pose.position.y-pose.pose.position.y)*(local_pos.pose.position.y-pose.pose.position.y) );
// 				ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << distance);
				
				
// 				if(distance<15){
// 					// serialPort.write(msg.data);
// 					std::exit(0);
// 				}
// 			break;
// }
// 		ros::spinOnce();
// 		rate.sleep();
// 		}}


// // 2024.7.20  successfully bombing in offboard mode
// #include <ros/ros.h>
// #include <std_msgs/String.h>
// #include <cmath>
// #include <string.h>
// #include <mavros_msgs/CommandBool.h>
// #include <mavros_msgs/SetMode.h>
// #include <mavros_msgs/State.h>
// #include <geometry_msgs/PoseStamped.h>
// #include <geometry_msgs/TwistStamped.h>

// #include <fstream>
// #include <sstream>
// #include <iostream>
// #include <string>
// #include <vector>
// #include <serial/serial.h>
// #include <time.h>
// #include <string>
// enum mode
// {
// 	mission = 0,
// 	offboard = 1,
// 	choose = 2,
// 	bombing = 3,
// };
// mode mission_mode = mission;

// mavros_msgs::State current_state;
// geometry_msgs::PoseStamped local_pos;
// geometry_msgs::TwistStamped local_v;

// void state_sb(const mavros_msgs::State::ConstPtr& msg) {
// 	current_state = *msg;
// 	ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << current_state);
// }
// void local_pos_sb(const geometry_msgs::PoseStamped::ConstPtr& msg) {
// 	local_pos = *msg;
	
// }
// void local_v_sb(const geometry_msgs::TwistStamped::ConstPtr& msg)
// {
// 	local_v = *msg;
// }
// std::string port = "/dev/ttyACM0"; // 根据实际情况修改串口设备号
// int baudrate = 9600;
// serial::Serial serialPort(port, baudrate, serial::Timeout::simpleTimeout(1000));



// int main(int argc, char** argv) {
//     ros::init(argc, argv, "mission_bombing");
// 	ros::NodeHandle nh;
//     ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>("/mavros/state", 10, state_sb);
// 	ros::Subscriber plane_local_pos_sub = nh.subscribe<geometry_msgs::PoseStamped>("/mavros/local_position/pose", 10, local_pos_sb);
// 	ros::Subscriber plane_local_v_sub = nh.subscribe<geometry_msgs::TwistStamped>("/mavros/local_position/velocity_local", 10, local_v_sb);

// 	ros::Publisher target_pos_pub = nh.advertise<geometry_msgs::PoseStamped>("/mavros/setpoint_position/local", 10);
// 	ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>("/mavros/cmd/arming");
// 	ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("/mavros/set_mode");
// 	ros::Rate rate(60.0);
// 	ros::spinOnce();
// 	rate.sleep();
	
// 	geometry_msgs::PoseStamped pose;
// 	pose.pose.position.x =-42;
// 	pose.pose.position.y =-10;
// 	pose.pose.position.z = 20;
// 	int i2=0;
	
	
//     while (ros::ok() && !current_state.connected)
// 	{
// 						//serialPort.write(std::to_string((i2++)%60));
// 						// serialPort.write("0");
						
// 	//	ROS_ERROR(("SERVO SPINING 0!!!!"));
// 		ROS_INFO("not connected");
// 		ros::spinOnce();
// 		rate.sleep();
// 	}
// 	for (int i = 100; ros::ok() && i > 0; --i) {
// 		pose.header.stamp = ros::Time::now();
// 		target_pos_pub.publish(pose);
// 		ros::spinOnce();
// 		rate.sleep();
// 	}
// 	mavros_msgs::SetMode offb_set_mode;
// 	offb_set_mode.request.custom_mode = "AUTO.MISSION";
// 	mavros_msgs::CommandBool arm_cmd;
// 	arm_cmd.request.value = true;
// 	ros::Time last_request = ros::Time::now();

//     // std_msgs::String msg;
// 	// msg.data = '1\n';//舵机控制

//     int ok=0;
// 	float distance=100;
// 	bool mission_status=1;

// 	time_t timep;
// 	 time_t timep2;
// time_t timep3;

//     while (ros::ok()) {


//         if (current_state.mode == "STABILIZED") 
//         {
// 			ROS_INFO("switch to STABILIZED");
			
// 			std::exit(0);
// 		}
//         switch (mission_mode) {
//         case 0: 
// 	// serialPort.write("59\n")
// 						// serialPort.write(std::to_string((i2++)%60));
// 						// serialPort.write("0");
// 					ROS_ERROR(("SERVO SPINING 0!!!!"));
//               if (!current_state.armed) 
//               {
//                   pose.header.stamp = ros::Time::now();
//                   if (arming_client.call(arm_cmd) && arm_cmd.response.success)
//                   {
//                       ROS_INFO("Vehicle armed");
//                    }
//                   last_request = ros::Time::now();
// 					time(&timep);
//               }
//               else if (current_state.armed&&mission_status) 
//               {
//                   if (current_state.mode != "AUTO.MISSION") 
//                   {
//                       pose.header.stamp = ros::Time::now();
                      
//                       if (set_mode_client.call(offb_set_mode) && offb_set_mode.response.mode_sent) 
//                       {
//                           ROS_INFO("Mission enabled");
// 						//   ROS_WARN(&timep);
// 						//printf("not move:timep111111:%d\n",timep);
                          
//                       }
//                   }
//               }
		
// 			  time(&timep2);
// 			//   ROS_ERROR(&timep2);
// 						//printf("move:timep22222:%d\n",timep2);

//             //    if(timep2-timep>30)
// 			std::cout<<"HEIGHT"<<local_pos.pose.position.z<<"\n";
// 			if (local_pos.pose.position.z>42)
//               {	offb_set_mode.request.custom_mode = "OFFBOARD";mission_status=0;

// 				if(set_mode_client.call(offb_set_mode)&&offb_set_mode.response.mode_sent){
                   
// 				  target_pos_pub.publish(pose);
// 				  ROS_INFO("offboard enabled");
// 				  mission_mode = offboard;
// 				  }
//               }
// 			break;
// 			case 1:
// 				distance =sqrt((local_pos.pose.position.x-pose.pose.position.x)*(local_pos.pose.position.x-pose.pose.position.x)+(local_pos.pose.position.y-pose.pose.position.y)*(local_pos.pose.position.y-pose.pose.position.y) );
// // distance=sqrt()
// 			    // if(distance>15){
// 					time(&timep3);
// 					// ROS_ERROR(&timep2);
// 					// ROS_FATAL(&timep3);
// 						//printf("not move :timep222222:%d\n",timep2);
// 						//printf("move:timep333333:%d\n",timep3);

// 						if (distance>33){
// 							target_pos_pub.publish(pose);
// 							ROS_INFO("offboard enabled111");
// 							ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << distance);
							
// 							}
// 						else 
// 						{
// 								ROS_WARN("IN >30");
// 								offb_set_mode.request.custom_mode = "AUTO.MISSION";
// 								if (set_mode_client.call(offb_set_mode) && offb_set_mode.response.mode_sent) 
// 								{
// 									ROS_WARN("IN >30 AND MODEL MISSION SENT");
// 									ROS_INFO("Mission enabled222");
// 									ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << distance);
// 									mission_mode = bombing;
// 								}						 
// 						}
// 				// }
// 						ROS_WARN("OUT OF >30 ,IN CASE1");
// 				break;
// 			case 3:
// 				distance =sqrt((local_pos.pose.position.x-pose.pose.position.x)*(local_pos.pose.position.x-pose.pose.position.x)+(local_pos.pose.position.y-pose.pose.position.y)*(local_pos.pose.position.y-pose.pose.position.y) );
// 				ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << distance);
// 				// if(distance<20){
// 				for(int i=1;i<=1000;i++)
// 					 serialPort.write("1\n");
// 					// ROS_ERROR("SERVO SPINING !180!!!!");
// 					std::exit(0);
// 				// }
// 			break;
// }
// 		ros::spinOnce();
// 		rate.sleep();
// }}




// // bombing test on land
/*
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <cmath>
#include <string.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TwistStamped.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <serial/serial.h>
#include <time.h>
#include <string>

mavros_msgs::State current_state;
geometry_msgs::PoseStamped local_pos;

void state_sb(const mavros_msgs::State::ConstPtr& msg) {
	current_state = *msg;
}
void local_pos_sb(const geometry_msgs::PoseStamped::ConstPtr& msg) {
	local_pos = *msg;
	ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << local_pos);
}
std::string port = "/dev/ttyACM0"; // 根据实际情况修改串口设备号
int baudrate = 9600;
serial::Serial serialPort(port, baudrate, serial::Timeout::simpleTimeout(1000));



int main(int argc, char** argv) {
    ros::init(argc, argv, "mission_bombing");
	ros::NodeHandle nh;
    ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>("/mavros/state", 10, state_sb);
	ros::Subscriber plane_local_pos_sub = nh.subscribe<geometry_msgs::PoseStamped>("/mavros/local_position/pose", 10, local_pos_sb);
		ros::Publisher target_pos_pub = nh.advertise<geometry_msgs::PoseStamped>("/mavros/setpoint_position/local", 10);
	ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>("/mavros/cmd/arming");
	ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("/mavros/set_mode");
	ros::Rate rate(60.0);
	ros::spinOnce();
	rate.sleep();
	
	geometry_msgs::PoseStamped pose;
	pose.pose.position.x = -31.0;
	pose.pose.position.y = -12.0;
	pose.pose.position.z = 25;
	double distance = 0;
    while (ros::ok() && !current_state.connected)
	{			
	//	ROS_ERROR(("SERVO SPINING 0!!!!"));
		ROS_INFO("not connected");
		ros::spinOnce();
		rate.sleep();
	}
	for (int i = 100; ros::ok() && i > 0; --i) {
		pose.header.stamp = ros::Time::now();
		ros::spinOnce();
		rate.sleep();
	}
	ros::Time last_request = ros::Time::now();
	while(ros::ok()){
		distance =sqrt((local_pos.pose.position.x-pose.pose.position.x)*(local_pos.pose.position.x-pose.pose.position.x)+(local_pos.pose.position.y-pose.pose.position.y)*(local_pos.pose.position.y-pose.pose.position.y) );
		ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << distance);
		if(distance <20 ){
			// bombing
			ROS_INFO("shoot");
			for(int i=1;i<=100;i++) {serialPort.write("1\n");}
			exit(0);
		}
		ros::spinOnce();
 		rate.sleep();
	}
}*/





//  wait 2s after bombing before returning mission mode
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <cmath>
#include <string.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TwistStamped.h>
#include <mavros_msgs/GPSRAW.h>
#include <nav_msgs/Odometry.h>
#include <mavros_msgs/GPSRAW.h>
#include <mavros_msgs/Altitude.h>
#include <mavros_msgs/VFR_HUD.h>
#include <mavros_msgs/Thrust.h>
#include <std_msgs/Float64.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <serial/serial.h>
#include <time.h>
#include <string>

 
int ok=0;
float distance=100;
bool mission_status=1;


enum mode
{
	mission = 0,
	offboard = 1,
	choose = 2,
	bombing = 3,
};
mode mission_mode = mission;

mavros_msgs::State current_state;
geometry_msgs::PoseStamped local_pos;
geometry_msgs::TwistStamped local_v;
geometry_msgs::TwistStamped local_v_body;
geometry_msgs::TwistStamped gpsvel;
nav_msgs::Odometry local_pos_odom;
nav_msgs::Odometry global_pos_odom;
mavros_msgs::GPSRAW gpsraw;
mavros_msgs::Altitude mav_alt;
std_msgs::Float64 rel_alt;
mavros_msgs::VFR_HUD hud;
void state_sb(const mavros_msgs::State::ConstPtr& msg) {
	current_state = *msg;
	// ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << current_state);
}
void local_pos_sb(const geometry_msgs::PoseStamped::ConstPtr& msg) {
	local_pos = *msg;
	
}
void local_v_sb(const geometry_msgs::TwistStamped::ConstPtr& msg)
{
	local_v = *msg;
}
void local_v_body_sb(const geometry_msgs::TwistStamped::ConstPtr& msg)
{
	local_v_body = *msg;
}
void local_pos_odom_sb(const nav_msgs::Odometry::ConstPtr& msg)
{
	local_pos_odom = *msg;
}
void global_pos_odom_sb(const nav_msgs::Odometry::ConstPtr& msg)
{
	global_pos_odom = *msg;
}
void gpsraw_sb(const mavros_msgs::GPSRAW::ConstPtr& msg)
{
	gpsraw = *msg;
}
void gpsvel_sb(const geometry_msgs::TwistStamped::ConstPtr& msg)
{
	gpsvel = *msg;
}
void mav_alt_sb(const mavros_msgs::Altitude::ConstPtr& msg)
{
	mav_alt = *msg;
}
void rel_alt_sb(const std_msgs::Float64::ConstPtr& msg)
{
	rel_alt = *msg;
}
void hud_sb(const mavros_msgs::VFR_HUD::ConstPtr& msg)
{
	hud = *msg;
}

std::string port = "/dev/ttyUSB0"; // 根据实际情况修改串口设备号
int baudrate = 9600;
serial::Serial serialPort(port, baudrate, serial::Timeout::simpleTimeout(1000));
// void printa(const std::ofstream &data,std::string  ch){

	// data<<""
	// <<"\n\t>>distance="<<distance
	// <<"\n\t>>mission_status"<<mission_status
	// <<"\n\t>>local pos = ("<<local_pos.pose.position.x<<",\t"<<local_pos.pose.position.y<<",\t"<<local_pos.pose.position.z<<")"
	// <<"\n\t>>local_v="<<local_v
	// <<"\n\t>>body
	// <<"\n\t>>local_pos_odom"<<local_pos_odom
	// <<"\n\t>>gpsraw="<<gpsraw
	// <<"\n";
// }

//airspeed groundspeed gps_alt baro_alt nav_orientation self_vel_body picth/roll/yaw  off:in->out
//
int main(int argc, char** argv) {
	// for(int i=1;i<=100;i++)
	// 	 serialPort.write("1\n");
	// return 0;

static std::time_t now = std::time(nullptr);
char buffer[80];
std::strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", std::localtime(&now));
std::string filename = "record_" + std::string(buffer);
std::ofstream data(filename);
	// for(int i=1;i<=1000;i++) {serialPort.write("1\n");}
    ros::init(argc, argv, "mission_bombing");
	ros::NodeHandle nh;
    ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>("/mavros/state", 10, state_sb);
	ros::Subscriber plane_local_pos_sub = nh.subscribe<geometry_msgs::PoseStamped>("/mavros/local_position/pose", 1, local_pos_sb);
	ros::Subscriber plane_local_v_sub = nh.subscribe<geometry_msgs::TwistStamped>("/mavros/local_position/velocity_local", 10, local_v_sb);
	ros::Subscriber plane_local_v_sub_body = nh.subscribe<geometry_msgs::TwistStamped>("/mavros/local_position/velocity_body", 10, local_v_body_sb);
	ros::Subscriber plane_gpsvel = nh.subscribe<geometry_msgs::TwistStamped>("/mavros/global_position/raw/gps_vel", 10, gpsvel_sb);
	ros::Subscriber plane_local_pos_odom = nh.subscribe<nav_msgs::Odometry>("/mavros/local_position/odom", 10, local_pos_odom_sb);
	ros::Subscriber plane_global_pos_odom = nh.subscribe<nav_msgs::Odometry>("/mavros/global_position/local", 10, global_pos_odom_sb);
    ros::Subscriber plane_gpsraw = nh.subscribe<mavros_msgs::GPSRAW>("/mavros/gpsstatus/gps1/raw", 10, gpsraw_sb);
	ros::Subscriber plane_mav_alt = nh.subscribe<mavros_msgs::Altitude>("/mavros/altitude", 10, mav_alt_sb);
	ros::Subscriber plane_rel_alt = nh.subscribe<std_msgs::Float64>("/mavros/global_position/rel_alt", 10, rel_alt_sb);
	ros::Subscriber plane_hud = nh.subscribe<mavros_msgs::VFR_HUD>("mavros_msgs/VFR_HUD", 10, hud_sb);
	// /mavros/setpoint_velocity/cmd_vel
	ros::Publisher target_pos_pub = nh.advertise<geometry_msgs::PoseStamped>("/mavros/setpoint_position/local", 10);
	ros::Publisher thrust_pub = nh.advertise<mavros_msgs::Thrust>("/mavros/setpoint_attitude/thrust", 10);
	ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>("/mavros/cmd/arming");
	ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("/mavros/set_mode");
	ros::Rate rate(60.0);
	ros::spinOnce();
	rate.sleep();
	
	geometry_msgs::PoseStamped pose;
	mavros_msgs::Thrust thrust;
	pose.pose.position.x =-36.8;
	pose.pose.position.y =-12.9;
	pose.pose.position.z = 20;
	int i2=0;
	
	//for(int i=1;i<=1000;i++) {serialPort.write("1\n");}


    while (ros::ok() && !current_state.connected)
	{
						//serialPort.write(std::to_string((i2++)%60));
						// serialPort.write("0");
						
	//	ROS_ERROR(("SERVO SPINING 0!!!!"));
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

    // std_msgs::String msg;
	// msg.data = '1\n';//舵机控制

    ok=0;
	distance=100;
	mission_status=1;

	time_t timep;
	time_t timep2;
	time_t timep3;

    while (ros::ok()) {
	data<<"{{{in while}}}"
	<<"\n\t>>distance="<<distance
	<<"\n\t>>mission_status"<<mission_status
	<<"\n\t>>local pos        	=("<<local_pos.pose.position.x<<",\t"<<local_pos.pose.position.y<<",\t"<<local_pos.pose.position.z<<")"
	<<"\n\t>>local_odom    =("<<local_pos_odom.pose.pose.position.x<<",\t"<<local_pos_odom.pose.pose.position.y<<",\t"<<local_pos_odom.pose.pose.position.z<<")"
	<<"\n\t>>global_odom =("<<global_pos_odom.pose.pose.position.x<<",\t"<<global_pos_odom.pose.pose.position.y<<",\t"<<global_pos_odom.pose.pose.position.z<<")"
	<<"\n\t>>gpsraw            	 =("<<gpsraw.alt*0.0004458936-4<<")"
	<<"\n\t>>mav_alt            =(local:"<<mav_alt.local<<"\trelative:"<<mav_alt.relative
	<<"\n\t>>rel_alt                =("<<rel_alt.data<<")"
	<<"\n\t>>local_v              =("<<local_v.twist.linear.x<<",\t"<<local_v.twist.linear.y<<",\t"<<local_v.twist.linear.z<<")"
	<<"\n\t>>body_v             =("<<local_v_body.twist.linear.x<<",\t"<<local_v_body.twist.linear.y<<",\t"<<local_v_body.twist.linear.z<<")"
	<<"\n\t>>gpsvel               =("<<gpsvel.twist.linear.x<<",\t"<<gpsvel.twist.linear.y<<",\t"<<gpsvel.twist.linear.z<<")"
	<<"\n\t>>hub_airspd    =(airspeed:"<<hud.airspeed<<",\tgroundspeed:"<<hud.groundspeed<<",\talt:"<<hud.altitude<<")"
	<<"\n";
	std::cout<<"{{{in while}}}"
	<<"\n\t>>distance="<<distance
	<<"\n\t>>mission_status"<<mission_status
	<<"\n\t>>local pos\t=("<<local_pos.pose.position.x<<",\t"<<local_pos.pose.position.y<<",\t"<<local_pos.pose.position.z<<")"
	<<"\n\t>>local_odom\t=("<<local_pos_odom.pose.pose.position.x<<",\t"<<local_pos_odom.pose.pose.position.y<<",\t"<<local_pos_odom.pose.pose.position.z<<")"
	<<"\n\t>>global_odom\t=("<<global_pos_odom.pose.pose.position.x<<",\t"<<global_pos_odom.pose.pose.position.y<<",\t"<<global_pos_odom.pose.pose.position.z<<")"
	<<"\n\t>>gpsraw\t=("<<gpsraw.alt*0.0004458936-4<<")"
	<<"\n\t>>mav_alt\t=(local:"<<mav_alt.local<<"\trelative:"<<mav_alt.relative<<")"
	<<"\n\t>>rel_alt\t=("<<rel_alt.data<<")"
	<<"\n\t>>local_v\t=("<<local_v.twist.linear.x<<",\t"<<local_v.twist.linear.y<<",\t"<<local_v.twist.linear.z<<")"
	<<"\n\t>>body_v\t=("<<local_v_body.twist.linear.x<<",\t"<<local_v_body.twist.linear.y<<",\t"<<local_v_body.twist.linear.z<<")"
	<<"\n\t>>gpsvel\t=("<<gpsvel.twist.linear.x<<",\t"<<gpsvel.twist.linear.y<<",\t"<<gpsvel.twist.linear.z<<")"
	<<"\n\t>>hub_airspd\t=(airspeed:"<<hud.airspeed<<",\tgroundspeed:"<<hud.groundspeed<<",\talt:"<<hud.altitude<<")"
	<<"\n\n\n\n";
        if (current_state.mode == "STABILIZED") 
        {
			ROS_INFO("switch to STABILIZED");
			
			std::exit(0);
		}
        switch (mission_mode) {
        case 0: 
	// serialPort.write("59\n")
						// serialPort.write(std::to_string((i2++)%60));
						// serialPort.write("0");
					// ROS_ERROR(("SERVO SPINING 0!!!!"));
              if (!current_state.armed) 
              {
                  pose.header.stamp = ros::Time::now();
                  if (arming_client.call(arm_cmd) && arm_cmd.response.success)
                  {
                      ROS_INFO("Vehicle armed");
                   }
                  last_request = ros::Time::now();
					time(&timep);
              }
              else if (current_state.armed&&mission_status) 
              {
                  if (current_state.mode != "AUTO.MISSION") 
                  {
                      pose.header.stamp = ros::Time::now();
                      
                      if (set_mode_client.call(offb_set_mode) && offb_set_mode.response.mode_sent) 
                      {
                          ROS_INFO("Mission enabled");
						//   ROS_WARN(&timep);
						//printf("not move:timep111111:%d\n",timep);
                          
                      }
                  }
              }
		
			  time(&timep2);
			//   ROS_ERROR(&timep2);
						//printf("move:timep22222:%d\n",timep2);

            //    if(timep2-timep>30)
			// std::cout<<"HEIGHT"<<local_pos.pose.position.z<<"\n";
			if (local_pos.pose.position.z>42)

              {	offb_set_mode.request.custom_mode = "OFFBOARD";mission_status=0;

				if(set_mode_client.call(offb_set_mode)&&offb_set_mode.response.mode_sent){
                   
				  target_pos_pub.publish(pose);
				  ROS_INFO("offboard enabled");
				  mission_mode = offboard;
				  }
              }
			break;
			case 1:

				distance =sqrt((local_pos.pose.position.x-pose.pose.position.x)*(local_pos.pose.position.x-pose.pose.position.x)+(local_pos.pose.position.y-pose.pose.position.y)*(local_pos.pose.position.y-pose.pose.position.y) );
				// distance=sqrt()
			    // if(distance>15){
					time(&timep3);
					// ROS_ERROR(&timep2);
					// ROS_FATAL(&timep3);
						//printf("not move :timep222222:%d\n",timep2);
						//printf("move:timep333333:%d\n",timep3);

						if (distance>33){
							data<<"now distance >30\n";
							target_pos_pub.publish(pose);
							ROS_INFO("offboard enabled111");
							ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" <<"{{{out of 30}}}"
	<<"\n\t>>distance="<<distance
	<<"\n\t>>mission_status"<<mission_status
	<<"\n\t>>local pos        	=("<<local_pos.pose.position.x<<",\t"<<local_pos.pose.position.y<<",\t"<<local_pos.pose.position.z<<")"
	<<"\n\t>>local_odom    =("<<local_pos_odom.pose.pose.position.x<<",\t"<<local_pos_odom.pose.pose.position.y<<",\t"<<local_pos_odom.pose.pose.position.z<<")"
	<<"\n\t>>global_odom =("<<global_pos_odom.pose.pose.position.x<<",\t"<<global_pos_odom.pose.pose.position.y<<",\t"<<global_pos_odom.pose.pose.position.z<<")"
	<<"\n\t>>gpsraw            	 =("<<gpsraw.alt*0.0004458936-4<<")"
	<<"\n\t>>mav_alt            =(local:"<<mav_alt.local<<"\trelative:"<<mav_alt.relative
	<<"\n\t>>rel_alt                =("<<rel_alt.data<<")"
	<<"\n\t>>local_v              =("<<local_v.twist.linear.x<<",\t"<<local_v.twist.linear.y<<",\t"<<local_v.twist.linear.z<<")"
	<<"\n\t>>body_v             =("<<local_v_body.twist.linear.x<<",\t"<<local_v_body.twist.linear.y<<",\t"<<local_v_body.twist.linear.z<<")"
	<<"\n\t>>gpsvel               =("<<gpsvel.twist.linear.x<<",\t"<<gpsvel.twist.linear.y<<",\t"<<gpsvel.twist.linear.z<<")"
	<<"\n\t>>hub_airspd    =(airspeed:"<<hud.airspeed<<",\tgroundspeed:"<<hud.groundspeed<<",\talt:"<<hud.altitude<<")"
	<<"\n";);
							}
						else 
						{		
							// thrust_pub.
					data<<"distance < 30\n";
								distance =sqrt((local_pos.pose.position.x-pose.pose.position.x)*(local_pos.pose.position.x-pose.pose.position.x)+(local_pos.pose.position.y-pose.pose.position.y)*(local_pos.pose.position.y-pose.pose.position.y) );
								// ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << distance<<"    local_v="<<v);
								// ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << local_v);
									data<<"{{{about to bombing}}}"
	<<"\n\t>>distance="<<distance
	<<"\n\t>>mission_status"<<mission_status
	<<"\n\t>>local pos        	=("<<local_pos.pose.position.x<<",\t"<<local_pos.pose.position.y<<",\t"<<local_pos.pose.position.z<<")"
	<<"\n\t>>local_odom    =("<<local_pos_odom.pose.pose.position.x<<",\t"<<local_pos_odom.pose.pose.position.y<<",\t"<<local_pos_odom.pose.pose.position.z<<")"
	<<"\n\t>>global_odom =("<<global_pos_odom.pose.pose.position.x<<",\t"<<global_pos_odom.pose.pose.position.y<<",\t"<<global_pos_odom.pose.pose.position.z<<")"
	<<"\n\t>>gpsraw            	 =("<<gpsraw.alt*0.0004458936-4<<")"
	<<"\n\t>>mav_alt            =(local:"<<mav_alt.local<<"\trelative:"<<mav_alt.relative
	<<"\n\t>>rel_alt                =("<<rel_alt.data<<")"
	<<"\n\t>>local_v              =("<<local_v.twist.linear.x<<",\t"<<local_v.twist.linear.y<<",\t"<<local_v.twist.linear.z<<")"
	<<"\n\t>>body_v             =("<<local_v_body.twist.linear.x<<",\t"<<local_v_body.twist.linear.y<<",\t"<<local_v_body.twist.linear.z<<")"
	<<"\n\t>>gpsvel               =("<<gpsvel.twist.linear.x<<",\t"<<gpsvel.twist.linear.y<<",\t"<<gpsvel.twist.linear.z<<")"
	<<"\n\t>>hub_airspd    =(airspeed:"<<hud.airspeed<<",\tgroundspeed:"<<hud.groundspeed<<",\talt:"<<hud.altitude<<")"
	<<"\n";
								for(int i=1;i<=50;i++) {serialPort.write("1\n");}
								ROS_INFO("IN >30 SERVO SPINING");
								ros::Duration(2).sleep();
								data<<"mode=1:write servo spin  after 2 second\n";
								offb_set_mode.request.custom_mode = "AUTO.MISSION";
								if (set_mode_client.call(offb_set_mode) && offb_set_mode.response.mode_sent) 
								{
								data<<"mode=1:back to mission\n";
									ROS_WARN("IN >30 AND MODEL MISSION SENT");
									ROS_INFO("Mission enabled222");
									// ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << distance);
									mission_mode = bombing;
								}						 
						}
				// }
						ROS_WARN("OUT OF >30 ,IN CASE1");
				break;
			case 3:
				distance =sqrt((local_pos.pose.position.x-pose.pose.position.x)*(local_pos.pose.position.x-pose.pose.position.x)+(local_pos.pose.position.y-pose.pose.position.y)*(local_pos.pose.position.y-pose.pose.position.y) );
				// ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << distance);
				// if(distance<20){
				// for(int i=1;i<=1000;i++)
				// 	 serialPort.write("1\n");
				// ROS_ERROR("SERVO SPINING !180!!!!");
				ROS_INFO("mission mode22222");
				std::exit(0);
				// }
			break;
}
		ros::spinOnce();
		rate.sleep();
}}
