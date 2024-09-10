#ifndef _header_h
#define _header_h

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <cmath>
#include <string.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <mavros_msgs/Trajectory.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TwistStamped.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <serial/serial.h>
#include <mavros_msgs/Waypoint.h>
#include <mavros_msgs/WaypointPush.h>
#include <mavros_msgs/WaypointClear.h>

enum mode
{
	mission_1 = 0,
	offboard = 1,
	bombing = 2,
	mission_2 = 3
};
mode mission_mode = mission_1;

mavros_msgs::State current_state;
geometry_msgs::PoseStamped local_pos;
geometry_msgs::TwistStamped local_vel;
constexpr float bombimg_height = 20.0;
constexpr float bombing_distance = 5;

mavros_msgs::Waypoint create_waypoint(float x, float y, float z, bool current, bool autocontinue){
    mavros_msgs::Waypoint wp;
    wp.frame = mavros_msgs::Waypoint::FRAME_LOCAL_ENU;
    wp.command = 16;
    wp.is_current = current;
    wp.autocontinue = autocontinue;
    wp.param1 = 0;
    wp.param2 = 0;
    wp.param3 = 0;
    wp.param4 = 0;
    wp.x_lat = x;
    wp.y_long = y;
    wp.z_alt = z;
    return wp;
}

// std::string port = "/dev/ttyUSB0"; // 根据实际情况修改串口设备号
// int baudrate = 115200; // 根据实际情况修改波特率
// serial::Serial serialPort(port, baudrate, serial::Timeout::simpleTimeout(1000));

#endif
