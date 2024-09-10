#include "mission_bombing/header.h"
#include "ros/ros.h"

void state_sb(const mavros_msgs::State::ConstPtr& msg) {
	current_state = *msg;
	ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << current_state);
}
void local_pos_sb(const geometry_msgs::PoseStamped::ConstPtr& msg) {
	local_pos = *msg;
}
void local_vel_sb(const geometry_msgs::TwistStamped::ConstPtr& msg) {
	local_vel = *msg;
}
std::string port = "/dev/ttyACM0"; // 根据实际情况修改串口设备号
int baudrate = 9600;
serial::Serial serialPort(port, baudrate, serial::Timeout::simpleTimeout(1000));

int main(int argc, char** argv) {
    ros::init(argc, argv, "mission_bombing");
	ros::NodeHandle nh;
    ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>("/plane_0/mavros/state", 10, state_sb);
	ros::Subscriber plane_local_pos_sub = nh.subscribe<geometry_msgs::PoseStamped>("/plane_0/mavros/local_position/pose", 1, local_pos_sb);
	ros::Subscriber plane_vel_pos_sub = nh.subscribe<geometry_msgs::TwistStamped>("/plane_0/mavros/local_position/velocity_local", 1, local_vel_sb);
	ros::Publisher trajectory_pub = nh.advertise<mavros_msgs::Trajectory>("/plane_0/mavros/trajectory/desired", 10);
	ros::Publisher target_pos_pub = nh.advertise<geometry_msgs::PoseStamped>("/plane_0/mavros/setpoint_position/local", 10);
	ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>("/plane_0/mavros/cmd/arming");
	ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("/plane_0/mavros/set_mode");
	ros::Rate rate(60.0);
	ros::spinOnce();
	rate.sleep();

	geometry_msgs::PoseStamped pose;
	pose.pose.position.x = 136.7;
	pose.pose.position.y = 150;
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

	mavros_msgs::SetMode set_mode;
	set_mode.request.custom_mode = "AUTO.MISSION";
	mavros_msgs::CommandBool arm_cmd;
	arm_cmd.request.value = true;
	ros::Time last_request = ros::Time::now();

	

    while (ros::ok()) {
		float distance = sqrt((local_pos.pose.position.x-pose.pose.position.x)*(local_pos.pose.position.x-pose.pose.position.x)+(local_pos.pose.position.y-pose.pose.position.y)*(local_pos.pose.position.y-pose.pose.position.y) );
        if (current_state.mode == "STABILIZED") 
        {
			ROS_INFO("switch to STABILIZED");
			std::exit(0);
		}
        switch (mission_mode) {
        case 0: 
			if (!current_state.armed) 
			{
                pose.header.stamp = ros::Time::now();
				if (arming_client.call(arm_cmd) && arm_cmd.response.success)
                   {
                      ROS_INFO("Vehicle armed");
                   }
                   last_request = ros::Time::now();
            }
            else if (current_state.armed && local_pos.pose.position.z < 34.5) // && ok < 10
            {
                if (current_state.mode != "AUTO.MISSION") 
                {
                    pose.header.stamp = ros::Time::now();
                    // target_pos_pub.publish(pose); 
                    if (set_mode_client.call(set_mode) && set_mode.response.mode_sent) 
                    {
                        ROS_INFO("Mission_1 enabled");
						// ROS_INFO("Variable type: %f", mission_mode);
                        // ok++;
                    }
                }
            }
            else if(current_state.armed && local_pos.pose.position.z >= 34.5) // && ok == 10
            {
                pose.header.stamp = ros::Time::now();
				if(current_state.mode != "OFFBOARD"){
					set_mode.request.custom_mode = "OFFBOARD";
					target_pos_pub.publish(pose);
					if(set_mode_client.call(set_mode) && set_mode.response.mode_sent)
					{
						mission_mode = offboard;
						// off_pos.pose.position = local_pos.pose.position;
						target_pos_pub.publish(pose);
						ROS_INFO("Offboard enabled");
						// ROS_INFO("Variable type: %f", mission_mode);
						// ok++;
					}
				}
				// else{
				// 	ROS_INFO("Preparing for Offboard...");
				// }
            }
			
			break;

			case 1:
				ros::spinOnce();
				
				if(distance > bombing_distance)
				{	
					ROS_INFO("Offboard mode");
					target_pos_pub.publish(pose);
				}
				else
				{	
					// bombing
					ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << distance);
					for(int i=1;i<=100;i++) {serialPort.write("1\n");}


					if (current_state.mode != "AUTO.MISSION") 
                	{
						set_mode.request.custom_mode = "AUTO.MISSION";
						pose.header.stamp = ros::Time::now();
						if (set_mode_client.call(set_mode) && set_mode.response.mode_sent) 
						{
							ROS_INFO("Mission_2 enabled");
							mission_mode = mission_2;
						}
					}
					
				}
			break;	
    			
			case 3:
				ros::spinOnce();
				ROS_INFO("MISSION22222");
			
			break;
		}
		ros::spinOnce();
		rate.sleep();
	}
}