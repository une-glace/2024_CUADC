#include <iostream>
#include <signal.h>
#include <stdlib.h>     /* srand, rand */
//#include <unistd.h>
#include <mutex>
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CompressedImage.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>

int main(int argc, char** argv) {
    ros::init(argc, argv, "video_publisher_node");
    ros::NodeHandle nh;
    
    ros::Publisher pub = nh.advertise<sensor_msgs::CompressedImage>("/video_stream", 10);
    ros::Rate rate(10);  // 设置发布频率为10Hz

    //cv::VideoCapture cap("/home/amov/catkin_ws1/src/tkdnn-ros/tkDNN/demo/yolo_test.mp4");  // 打开摄像头
    cv::VideoCapture cap(0);
     if (!cap.isOpened()) {
        ROS_ERROR("Failed to open camera.");
        return -1;
    }
    cv::Mat frame;
    int w = cap.get(cv::CAP_PROP_FRAME_WIDTH);
        int h = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
	std::cout << w << ' ' << h <<'\n';
    while (ros::ok()) {
        std::cout << w << ' ' << h <<'\n';
        cap >> frame;
        if(frame.rows==0) continue;
        // if (frame.empty()) {
        //     ROS_ERROR("Failed to capture frame.");
        //     break;
        // }

        // 转换图像为ROS支持的消息类型
        cv_bridge::CvImage cv_image;
        cv_image.image = frame;
        cv_image.encoding = "bgr8";
        sensor_msgs::ImagePtr ros_image = cv_image.toImageMsg();
	std::cout << '\n';
        // 发布图像消息
        pub.publish(ros_image);

        ros::spinOnce();
        rate.sleep();
    }

    cap.release();

    return 0;
}
