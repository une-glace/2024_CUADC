#include<ros/ros.h>
#include <string.h>
#include<cmath>
#include<mavros_msgs/CommandBool.h> //CommandBool·þÎñµÄÍ·ÎÄŒþ£¬žÃ·þÎñµÄÀàÐÍÎªmavros_msgs£º£ºCommandBool
#include <mavros_msgs/SetMode.h> //SetMode·þÎñµÄÍ·ÎÄŒþ£¬žÃ·þÎñµÄÀàÐÍÎªmavros_msgs£º£ºSetMode
#include <mavros_msgs/State.h>   //¶©ÔÄµÄÏûÏ¢ÌåµÄÍ·ÎÄŒþ£¬žÃÏûÏ¢ÌåµÄÀàÐÍÎªmavros_msgs£º£ºState
#include <geometry_msgs/PoseStamped.h> //·¢²ŒºÍ¶©ÔÄµÄÏûÏ¢Ìå¶ÔÓŠµÄÍ·ÎÄŒþ£¬žÃÏûÏ¢ÌåµÄÀàÐÍÎªgeometry_msgs£º£ºPoseStamped

// œšÁ¢Ò»žö¶©ÔÄÏûÏ¢ÌåÀàÐÍµÄ±äÁ¿£¬ÓÃÓÚŽæŽ¢¶©ÔÄµÄÐÅÏ¢,Í¬Ê±ÊµÊ±ÐÞžÄ²ÎÊý
mavros_msgs::State current_state;
geometry_msgs::PoseStamped  local_pos;

// œšÁ¢Ò»žö¹ŠÄÜmodeÃ¶ŸÙ£¬ÓÃÓÚµ÷Õû·ÉÐÐÄ£Êœ
enum mode {
	mission = 0,
	offboard = 1,
	land = 2,
	takeoff = 3,
	none = 4
};
mode mission_mode = mission;

// ¶©ÔÄ»Øµ÷º¯Êý£¬ÓÃÓÚ¶©ÔÄžüÐÂ×ŽÌ¬
void state_sb(const mavros_msgs::State::ConstPtr& msg) {
	current_state = *msg;
}
void local_pos_sb(const geometry_msgs::PoseStamped::ConstPtr& msg) {
	local_pos = *msg;
}


//Ö÷º¯Êý²¿·Ö
int main(int argc, char** argv) {
	ros::init(argc, argv, "fly"); // rosÏµÍ³µÄ³õÊŒ»¯£¬×îºóÒ»žö²ÎÊýÎªœÚµãÃû³Æ
	ros::NodeHandle nh;

	// ¶©ÔÄ¡£<>ÀïÃæÎªÄ£°å²ÎÊý£¬Ž«ÈëµÄÊÇ¶©ÔÄµÄÏûÏ¢ÌåÀàÐÍ£¬£š£©ÀïÃæŽ«ÈëÈýžö²ÎÊý£¬·Ö±ðÊÇžÃÏûÏ¢ÌåµÄÎ»ÖÃ¡¢»ºŽæŽóÐ¡£šÍš³£Îª1000£©¡¢»Øµ÷º¯Êý
	ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>("/mavros/state", 10, state_sb);
	ros::Subscriber plane_local_pos_sub = nh.subscribe<geometry_msgs::PoseStamped>("/mavros/local_position/pose", 1, local_pos_sb);


	// ·¢²Œ¡£ÓÃÓÚ·¢²ŒÄ¿±êºœµã£¬¿ØÖÆ·ÉÐÐ·œÏò£¬ÐÞžÄ·ÉÉè²ÎÊý
	ros::Publisher target_pos_pub = nh.advertise<geometry_msgs::PoseStamped>("/mavros/setpoint_position/local", 10);



	ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>("/mavros/cmd/arming");
	ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("/mavros/set_mode");


	ros::Rate rate(20.0);
	ros::spinOnce();
	rate.sleep();
	geometry_msgs::PoseStamped pose;
	geometry_msgs::PoseStamped pose_origin;
	pose.pose.position.x =0;
	pose.pose.position.y =100;
	pose.pose.position.z = 50;
	
	while (ros::ok() && !current_state.connected) {
		ROS_INFO("not connected");
		ros::spinOnce();
		rate.sleep();
	}
	for (int i = 100; ros::ok() && i > 0; --i) { // this step is to enable offboard because of the bug, has no meaning in our code{
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
	float distance = 0;
	int switchm = 0;


	while (ros::ok()) {
		//if(current_state.mode == "AUTO.MISSION"){switchm = 1;};
		//ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << current_state.mode);
		
		pose.header.stamp = ros::Time::now();
		target_pos_pub.publish(pose);
		//if(!switchm) continue;
		//if(switchm == 2)  break;
		if (current_state.mode == "STABILIZED")
		{
			ROS_INFO("switch to OFFBOARD");
			std::exit(0);
		}
		ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << local_pos);

		switch (mission_mode) {
			//ÈÎÎñÄ£Êœ¿ØÖÆÆð·ÉœµÂäÓÚœâËø
		case 0:  
			if (!current_state.armed && ok == 0) {
				pose.header.stamp = ros::Time::now();
				target_pos_pub.publish(pose);
				if (arming_client.call(arm_cmd) && arm_cmd.response.success)
				{
					ROS_INFO("Vehicle armed"); // œâËøºóŽòÓ¡ÐÅÏ¢
					
				}
				last_request = ros::Time::now();
			}
			else if (current_state.armed && ok == 0) {
				if (current_state.mode != "AUTO.MISSION") {
					pose.header.stamp = ros::Time::now();
					target_pos_pub.publish(pose);
					pose_origin.pose.position.x = local_pos.pose.position.x;
					pose_origin.pose.position.y = local_pos.pose.position.y;
					pose_origin.pose.position.z = local_pos.pose.position.z;
					pose.pose.position.z = 50+ pose_origin.pose.position.z;
					if (set_mode_client.call(offb_set_mode) && offb_set_mode.response.mode_sent) {
						ROS_INFO("Mission enabled"); // Žò¿ªÄ£ÊœºóŽòÓ¡ÐÅÏ¢
					}
				}
				if (cnt == 0 && local_pos.pose.position.z-pose_origin.pose.position.z > 40)
				{
					ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << pose);
					offb_set_mode.request.custom_mode = "OFFBOARD"; // TODO: add a judgement to guarantee the mode change succeed
					mission_mode = offboard;
					cnt++;
				}
			}
			else if (ok == 1 && local_pos.pose.position.z > 0) {
				if (current_state.mode != "AUTO.MISSION")
				{	pose.header.stamp = ros::Time::now();
					target_pos_pub.publish(pose);
					if (set_mode_client.call(offb_set_mode) && offb_set_mode.response.mode_sent) // TODO: wtf are you doing, separate takeoff and offboard
						ROS_INFO("Mission enabled"); // Žò¿ªÄ£ÊœºóŽòÓ¡ÐÅÏ¢  
					}
			}
			else if (ok == 1 && local_pos.pose.position.z <= 0) {
				ROS_INFO("Vehicle disarmed");
				ok == 2;
			}
			break;

			//offboardÄ£ÊœÓÃÓÚÖŽÐÐÈÎÎñ
		case 1:
			if (current_state.mode != "OFFBOARD")
			{
			
				if (set_mode_client.call(offb_set_mode) && offb_set_mode.response.mode_sent)
				{

					ROS_INFO("Offboard enabled"); // Žò¿ªÄ£ÊœºóŽòÓ¡ÐÅÏ¢
				}
				else
				{
					ROS_INFO("Offboard disabled"); // Žò¿ªÄ£ÊœºóŽòÓ¡ÐÅÏ¢
				}
			}

			//if (current_state.mode == "AUTO.LAND") switchm++;
			




			distance=sqrt((local_pos.pose.position.x - pose.pose.position.x) * (local_pos.pose.position.x - pose.pose.position.x) + (local_pos.pose.position.y - pose.pose.position.y) * (local_pos.pose.position.y - pose.pose.position.y));
				ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m \033[0m" << distance);
			ros::spinOnce();
			rate.sleep();
			pose.header.stamp = ros::Time::now();
			target_pos_pub.publish(pose);
			if (count == 0 && distance< 5)
			{	ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m complete1\033[0m");
				pose.pose.position.x = pose.pose.position.x ;
				pose.pose.position.y = pose.pose.position.y+200 ;
				count++;
				continue;
			}
			if (count == 1 && distance < 3)
			{
				ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m complete2\033[0m");
				pose.pose.position.x = pose.pose.position.x-100;
				pose.pose.position.y = pose.pose.position.y;
				count++;
				continue;
			}
			if (count == 2 && distance < 10)
			{
				ROS_INFO_STREAM_THROTTLE(1, "\033[1;32m complete3\033[0m");
				mission_mode = mission;
				offb_set_mode.request.custom_mode = "AUTO.MISSION";
				last_request = ros::Time::now();
				ok = 1;
				break;
			}
	
			break;
		default:
			last_request = ros::Time::now();
			break;
		}
		ros::spinOnce();
		rate.sleep();
	if (ok == 2) 
		break;
	}

	return 0;
}
