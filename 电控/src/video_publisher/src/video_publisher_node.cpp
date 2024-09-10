#include <iostream>
#include <ros/ros.h>
#include <sensor_msgs/CompressedImage.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>

int main(int argc, char** argv) {
    ros::init(argc, argv, "compressed_video_publisher_node");
    ros::NodeHandle nh;

    ros::Publisher pub = nh.advertise<sensor_msgs::CompressedImage>("/compressed_video_stream", 10000);
    ros::Rate rate(5);

    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        ROS_ERROR("Failed to open camera.");
        return -1;
    }
    cv::Mat frame;

    while (ros::ok()) {
        cap >> frame;
        if (frame.rows == 0) continue;

        // 压缩图像
        std::vector<uchar> jpeg_buffer;
        cv::imencode(".jpg", frame, jpeg_buffer);

        // 创建压缩图像消息
        sensor_msgs::CompressedImage compressed_image_msg;
        compressed_image_msg.format = "jpeg";
        compressed_image_msg.data = jpeg_buffer;

        // 发布压缩图像消息
        pub.publish(compressed_image_msg);
        //std::cout<<"published"<<std::endl;
        ros::spinOnce();
        rate.sleep();
    }

    cap.release();

    return 0;
}

