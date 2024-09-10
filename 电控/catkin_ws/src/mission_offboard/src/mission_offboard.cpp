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
#include <mavros_msgs/WaypointReached.h>
#include <mavros_msgs/RCIn.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <serial/serial.h>
#include <time.h>
#include <string>


 //用于控制任务状态的全局变量
int ok=0;
float distance=100;
bool mission_status=1;


//四种任务模式
enum mode
{
	mission = 0,
	offboard = 1,
	choose = 2,
	bombing = 3,
};
mode mission_mode = mission;


//存储从各个话题接收到的数据
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
mavros_msgs::WaypointReached wp;
mavros_msgs::RCIn rcin;


//回调函数用于更新订阅到的数据，每个回调函数与一个ROS话题相关联，用于接收状态、位置、速度、GPS数据等信息
void state_sb(const mavros_msgs::State::ConstPtr& msg) {
	current_state = *msg;
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
void wayRea_sb(const mavros_msgs::WaypointReached::ConstPtr& msg)
{
	wp = *msg;
}
void rcin_sb(const mavros_msgs::RCIn::ConstPtr& msg) 
{
	rcin = *msg;
}


//初始化串口，用于与外部设备通信，控制舵机
std::string port = "/dev/ttyUSB0"; // 根据实际情况修改串口设备号
int baudrate = 9600;
serial::Serial serialPort(port, baudrate, serial::Timeout::simpleTimeout(1000));


int main(int argc, char** argv) {
	 serialPort.write("70a");
    
	float x = 0, y = 0;
    
	std::cout << "pose.x: ";
    std::cin >> x;
    std::cout << "pose.y: ";
    std::cin >> y;

    if (x == 0 || y == 0) {
        // 读取文件中的这两个数
        std::ifstream inFile("coordinates.txt");
        if (inFile.is_open()) {
            inFile >> x >> y;
            inFile.close();
            std::cout << "Read from file: x = " << x << ", y = " << y << std::endl;
        } 
		else {
            std::cout << "Unable to open file";
        }
    } 
	else {
        // 把这新输入的两个数放入文件中
        std::ofstream outFile("coordinates.txt");
        if (outFile.is_open()) {
            outFile << x << " " << y;
            outFile.close();
            std::cout << "Coordinates written to file: x = " << x << ", y = " << y << std::endl;
        } 
		else {
            std::cout << "Unable to open file";
        }
    }


//测试舵机臂偏转角度
//servoservo
// for(int i=1;i<=30;i++)
// 	serialPort.write("0a");
 //serialPort.write("70\n");
 //return 0;


	//创建一个以当前时间为标识的日志文件
	static std::time_t now = std::time(nullptr);
	char buffer[80];
	std::strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", std::localtime(&now));
	std::string filename = "record_" + std::string(buffer);
	std::ofstream data(filename);


	//初始化ROS节点并设置订阅器和发布器
	ros::init(argc, argv, "mission_offboard");
	ros::NodeHandle nh;
	ros::Subscriber rcin_sub = nh.subscribe<mavros_msgs::RCIn>("/mavros/rc/in", 1, rcin_sb);
	ros::Subscriber wayRea_sub = nh.subscribe<mavros_msgs::WaypointReached>("/mavros/mission/reached", 1, wayRea_sb);
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
	ros::Publisher target_pos_pub = nh.advertise<geometry_msgs::PoseStamped>("/mavros/setpoint_position/local", 10);
	ros::Publisher thrust_pub = nh.advertise<mavros_msgs::Thrust>("/mavros/setpoint_attitude/thrust", 10);
	ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>("/mavros/cmd/arming");
	ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("/mavros/set_mode");
	ros::Rate rate(60.0);
	ros::spinOnce();
	rate.sleep();
	

	//姿态高度
	geometry_msgs::PoseStamped pose, delt_pose, target;
	mavros_msgs::Thrust thrust;
	
	
	//egeg
	pose.pose.position.x =x;
	pose.pose.position.y =y;
	pose.pose.position.z = 20;
	int i2=0;
	delt_pose.pose.position.x = local_pos.pose.position.x - global_pos_odom.pose.pose.position.x;
	delt_pose.pose.position.y = local_pos.pose.position.y - global_pos_odom.pose.pose.position.y;
	delt_pose.pose.position.z = local_pos.pose.position.z - global_pos_odom.pose.pose.position.z;


	while (ros::ok() && !current_state.connected){
		ROS_INFO("not connected");
		ros::spinOnce();
		rate.sleep();
	}
		

	//连续发布目标姿态信息（pose）
	for (int i = 100; ros::ok() && i > 0; --i) {
			pose.header.stamp = ros::Time::now();
			target_pos_pub.publish(pose);
			ros::spinOnce();
			rate.sleep();
	}


	//飞行模式的切换和解锁
	mavros_msgs::SetMode offb_set_mode;
	offb_set_mode.request.custom_mode = "AUTO.MISSION";
	mavros_msgs::CommandBool arm_cmd;
	arm_cmd.request.value = true;
	ros::Time last_request = ros::Time::now();


	ok=0;
	distance=100;
	mission_status=1;


	time_t timep;
	time_t timep2;
	time_t timep3;


	//检查文件是否是空的
	bool isFileEmpty(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    return file.tellg() == 0;
	}

	while (ros::ok()) {
		/*
		std::string filename =  "coordinates.txt";
		if (isFileEmpty(filename)){
			pose.pose.position.x = -40;
			pose.pose.position.y = -6;
		}
		else{
			std::ifstream inFile("coordinates.txt");
			if (inFile.is_open()) {
        	inFile >> pose.pose.position.x >> pose.pose.position.y;
        	inFile.close();
        	std::cout << "Read from file: x = " << pose.pose.position.x << ", y = " << pose.pose.position.y << std::endl;
    		} 
			else {
        		std::cout << "Unable to open file";
    		}
		}*/

		delt_pose.pose.position.x = local_pos.pose.position.x - global_pos_odom.pose.pose.position.x;
		delt_pose.pose.position.y = local_pos.pose.position.y - global_pos_odom.pose.pose.position.y;
		delt_pose.pose.position.z = local_pos.pose.position.z - global_pos_odom.pose.pose.position.z;
		target.pose.position.x = pose.pose.position.x + delt_pose.pose.position.x;
		target.pose.position.y = pose.pose.position.y + delt_pose.pose.position.y;
		target.pose.position.z = pose.pose.position.z + delt_pose.pose.position.z;
	
		//状态信息输出到日志文件
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
		<<"\n\t>>rc_in                 =(channels1:"<< rcin.channels[0] << ",\tchannels2:" <<rcin.channels[1]<<")"
		<<"\n";
		//状态信息输出到控制台
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
		<<"\n\t>>rc_in\t=(channels1:"<< rcin.channels[0] << ",\tchannels2:" <<rcin.channels[1]<<")"
		<<"\n\n\n\n";
        
		if (current_state.mode == "STABILIZED") {
			ROS_INFO("switch to STABILIZED");
			serialPort.write("70a");
			std::exit(0);
		}
        
		switch (mission_mode) {
        case 0: 
            if (!current_state.armed){
                pose.header.stamp = ros::Time::now();
                if (arming_client.call(arm_cmd) && arm_cmd.response.success){
                    ROS_INFO("Vehicle armed");
                }
                last_request = ros::Time::now();
				time(&timep);
            }
            else if (current_state.armed&&mission_status) {
                if (current_state.mode != "AUTO.MISSION") {
                    pose.header.stamp = ros::Time::now();
                    if (set_mode_client.call(offb_set_mode) && offb_set_mode.response.mode_sent){
                        ROS_INFO("Mission enabled");
		 			}
                }
            }
		
			time(&timep2);
			
			if  (wp.wp_seq >= 7){	
				if(current_state.mode == "OFFBOARD"){
					mission_mode = offboard;
					ROS_INFO("offboard enabled");
				}
				offb_set_mode.request.custom_mode = "OFFBOARD";
				mission_status=0;
				if(set_mode_client.call(offb_set_mode)&&offb_set_mode.response.mode_sent){
        			target_pos_pub.publish(target);
				}
			}
			break;
			
		case 1:
				distance = sqrt((local_pos.pose.position.x-target.pose.position.x)*(local_pos.pose.position.x-target.pose.position.x)+(local_pos.pose.position.y-target.pose.position.y)*(local_pos.pose.position.y-target.pose.position.y) );
				time(&timep3);
				
				if (distance > 37){
					data<<"now distance >37\n";
					target_pos_pub.publish(target);
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
				else {		
					data<<"distance < 37\n";
					distance =sqrt((local_pos.pose.position.x-target.pose.position.x)*(local_pos.pose.position.x-target.pose.position.x)+(local_pos.pose.position.y-target.pose.position.y)*(local_pos.pose.position.y-target.pose.position.y) );
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
					serialPort.write("0a");
					ROS_INFO("IN >30 SERVO SPINING");
					data<<"mode=1:write servo spin  after 2 second\n";
					offb_set_mode.request.custom_mode = "AUTO.MISSION";
					if (set_mode_client.call(offb_set_mode) && offb_set_mode.response.mode_sent) {
						data<<"mode=1:back to mission\n";
						ROS_WARN("IN >30 AND MODEL MISSION SENT");
						ROS_INFO("Mission enabled222");
						mission_mode = bombing;
					}						 
				}
				ROS_WARN("OUT OF >30 ,IN CASE1");
				break;
			
		case 3:
				distance =sqrt((local_pos.pose.position.x-target.pose.position.x)*(local_pos.pose.position.x-target.pose.position.x)+(local_pos.pose.position.y-target.pose.position.y)*(local_pos.pose.position.y-target.pose.position.y) );
				ROS_INFO("mission mode22222");
				break;
		}
		
		ros::spinOnce();
		rate.sleep();
	}
}
