#include <ros/ros.h>
#include <fstream>
#include <chrono>
#include <cstdlib> // For rand() and RAND_MAX
#include <ctime>   // For time() to seed the random number generator

int main(int argc, char** argv)
{
    ros::init(argc, argv, "write_to_txt_node");
    ros::NodeHandle nh;
    ros::Rate rate(40); 
for(int i=0;i<500;i++){
    // Seed the random number generator with the current time
     auto currentTime = std::chrono::high_resolution_clock::now();
    auto seed = static_cast<unsigned>(std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count());

    // 使用种子初始化随机数生成器
    std::srand(seed);

    // Generate three random float numbers between 0.0 and 1.0
    float data[] = {(float)std::rand() / RAND_MAX, (float)std::rand() / RAND_MAX, (float)std::rand() / RAND_MAX};

    // Open the file for writing
    std::ofstream file("/home/amov/Desktop/position.txt");
    if (file.is_open()) {
        // Write the data to the file
        for (int i = 0; i < sizeof(data) / sizeof(data[0]); i++) {
            file << data[i] << " ";
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
        file.close();
    } else {
        std::cerr << "Unable to open the file for writing." << std::endl;
    }

    rate.sleep();
    ros::spinOnce();
}
    return 0;
}

