#include <ros/ros.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "read_and_display");
    ros::NodeHandle nh;
      ros::Rate rate(20);
    for(int i=0;i<500;i++) {
        // Open the file for reading
        std::ifstream file("/home/amov/Desktop/position.txt");
        if (file.is_open()) {
            std::string line;
            if (std::getline(file, line)) {  // Read the first line
                std::istringstream iss(line);
                std::vector<float> data;
                float value;
                while (iss >> value) {
                    data.push_back(value);
                }
                // Check if we have exactly 3 float values
                if (data.size() == 3) {
                    // Display the read data
                    std::cout << "Read data: ";
                    for (const float& val : data) {
                        std::cout << val << " ";
                    }
                    std::cout << std::endl;
                } else {
                    ROS_ERROR("Invalid data format in the file. Expected 3 float values.");
                }
            } else {
                ROS_ERROR("Failed to read the first line from the file.");
            }
            file.close();
        } else {
            ROS_ERROR("Unable to open the file for reading.");
        }
	rate.sleep();
        ros::spinOnce();
    }

    return 0;
}

